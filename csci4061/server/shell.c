/* CSci4061 S2016 Assignment 2
* section: 1
* section: 5
* date: 03/11/16
* name: Yuanhao Lu, Phoebe Zhang, Peter Nguyen (for partner(s))
* id: luxx0267, zhan2937, nguy1435 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <signal.h>

#include "util.h"

/*
 * Read a line from stdin.
 */
char *sh_read_line(void)
{
	char *line = NULL;
	ssize_t bufsize = 0;

	getline(&line, &bufsize, stdin);
	return line;
}

/*
 * Do stuff with the shell input here.
 */
int sh_handle_input(char *line, int fd_toserver)
{
 	/* Check for \seg command and create segfault */
	if (starts_with(line, CMD_SEG)) {
		char *n = NULL;
		*n = 1;
	} 
	/* Write message to server for processing */
	else 
	{	if (is_empty(line) == 0)
		{
			if (write(fd_toserver, line, strlen(line)+1) < 0)
				perror("write to server failed");
		}
	}
	return 0;
}

/*
 * Check if the line is empty (no data; just spaces or return)
 */
int is_empty(char *line)
{
	while (*line != '\0') {
		if (!isspace(*line))
			return 0;
		line++;
	}
	return 1;
}

/*
 * Start the main shell loop:
 * Print prompt, read user input, handle it.
 */
void sh_start(char *name, int fd_toserver)
{
	char *line;

	while (1) 
	{	
		print_prompt(name);
		line = sh_read_line();	
		sh_handle_input(line, fd_toserver);	
	}
}

int main(int argc, char **argv)
{
	if(argc != 4)
	{
		printf("usage: shell fd_from_server fd_to_server name\n");
		exit(1);
	}
	
	/* Extract pipe descriptors and name from argv */
	int fd0 = atoi(argv[1]);
	int fd1 = atoi(argv[2]);
	
	char *name = argv[3];
	char buffer[MSG_SIZE];
	int i, nread, status;

	/* Fork a child to read from the pipe continuously */
	pid_t pid;
	pid = fork();	

	if (pid == 0) 
	{
		close (fd1);
		while (1) {	
			/* Let the CPU breathe */
			usleep(1000);	

			nread = read(fd0, buffer, MSG_SIZE);
			if (nread == -1 ) {
				if (errno == EAGAIN) {				
					fflush(stdout);					
				} else {
					fprintf(stderr, "ERROR: Can't read from pipe.\n");
					exit(1);
				}
			} else if ( nread == 0 ) {
				printf("Pipe closed, end of conversation\n");	
				fflush(stdout);
				exit(0);
			} else {
				printf("%s\n", buffer);	
				print_prompt(name);			
				fflush(stdout);			
			}
		}
		close(fd0);
		exit(1);
	}
	else if (pid > 0)
	{
		close(fd0);

		char text[MSG_SIZE];
		sprintf(text, "%s %d", CMD_CHILD_PID, pid);

		// Send child's pid to the server
		if (write(fd1, text, strlen(text)+1) < 0)
			perror("write to server failed");

		sh_start(name, fd1);
		close(fd1);
		wait(&status);	
	}
	else
	{
		perror("failed to fork:");
		exit(1);
	}
}
