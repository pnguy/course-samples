/* CSci4061 S2016 Assignment 2
* section: 1
* section: 5
* date: 03/11/16
* name: Yuanhao Lu, Phoebe Zhang, Peter Nguyen (for partner(s))
* id: luxx0267, zhan2937, nguy1435 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#include "util.h"

/*
 * Identify the command used at the shell
 */
int parse_command(char *buf)
{
	int cmd;

	if (starts_with(buf, CMD_CHILD_PID))
		cmd = CHILD_PID;
	else if (starts_with(buf, CMD_P2P))
		cmd = P2P;
	else if (starts_with(buf, CMD_LIST_USERS))
		cmd = LIST_USERS;
	else if (starts_with(buf, CMD_ADD_USER))
		cmd = ADD_USER;
	else if (starts_with(buf, CMD_EXIT))
		cmd = EXIT;
	else if (starts_with(buf, CMD_KICK))
		cmd = KICK;
	else
		cmd = BROADCAST;

	return cmd;
}

/*
 * List the existing users on the server shell
 */
int list_users(user_chat_box_t *users, int fd)
{

	int i;
	char const *no_users = "<no users>", *s;
	char active_users[MSG_SIZE] = "\n";

	// Create string of active users
	for (i = 0; i < MAX_USERS; i++) {
		if (users[i].status == SLOT_EMPTY)
			continue;
		sprintf(active_users, "%s\n%s", active_users, users[i].name);
	}

	if (strcmp(active_users, "\n") == 0) {
		if (write(fd, no_users, strlen(no_users) + 1) < 0)
			perror("writing to child shell failed");
	}
	else
	{
		if (write(fd, active_users, strlen(active_users) + 1) < 0)
			perror("writing to child shell failed");
	}

}

/*
 * Add a new user
 * Returns index of user else -1 if max user
 */
int add_user(user_chat_box_t *users, char *buf, int server_fd)
{
	int i;
	char const *full = "User not added, max user limit reached...";
	char const *msg = "Adding user";
	char text[MSG_SIZE];

	/* Check if user limit reached. */
	for (i = 0; i < MAX_USERS; i++)
	{
		if (users[i].status == SLOT_FULL)
		{
			if ((i+1) == MAX_USERS)
			{
				if (write(server_fd, full, strlen(full) + 1) < 0)
					perror("writing to server shell");
			}
			else
			{
				continue;
			}
		}
		else
		{
			strcpy(users[i].name, strdup(buf));
			users[i].status = SLOT_FULL;
			break;
		}
	}

	if (i < MAX_USERS)
	{
		// Set up non-blocking pipes and notify on server shell
		int p_c[2], c_p[2], flags;

		if (pipe(p_c) == -1 || pipe(c_p) == -1)
		{
			fprintf(stderr, "Call to pipe error.\n");
			exit(1);
		}

		flags = fcntl(p_c[0], F_GETFL, 0);
		if (fcntl(p_c[0], F_SETFL, flags | O_NONBLOCK) == -1 )
		{
			fprintf(stderr, "Call to fcntl error.\n");
			exit(1);
		}

		flags = fcntl(c_p[0], F_GETFL, 0);
		if (fcntl(c_p[0], F_SETFL, flags | O_NONBLOCK) == -1 )
		{
			fprintf(stderr, "Call to fcntl error.\n");
			exit(1);
		}

		users[i].ptoc[0] = p_c[0];
		users[i].ptoc[1] = p_c[1];
		users[i].ctop[0] = c_p[0];
		users[i].ctop[1] = c_p[1];

		/* Notify on server shell */
		sprintf(text, "%s %s...", msg, users[i].name);
		if (write(server_fd, text, strlen(text) + 1) < 0)
			perror("writing to server shell");

		return i;
	}

	return -1;
}

/*
 * Broadcast message to all users. Completed for you as a guide to help with other commands :-).
 */
int broadcast_msg(user_chat_box_t *users, char *buf, int fd, char *sender)
{
	int i;
	const char *msg = "Broadcasting...", *s;
	char text[MSG_SIZE];

	/* Notify on server shell */
	if (write(fd, msg, strlen(msg) + 1) < 0)
		perror("writing to server shell");

	/* Send the message to all user shells */
	s = strtok(buf, "\n");
	sprintf(text, "%s : %s", sender, s);
	for (i = 0; i < MAX_USERS; i++) {
		if (users[i].status == SLOT_EMPTY)
			continue;
		if (write(users[i].ptoc[1], text, strlen(text) + 1) < 0)
			perror("write to child shell failed");
	}
}

/*
 * Close all pipes for this user
 */
void close_pipes(int idx, user_chat_box_t *users)
{
	close(users[idx].ptoc[0]);
	close(users[idx].ptoc[1]);
	close(users[idx].ctop[0]);
	close(users[idx].ctop[1]);
}

/*
 * Cleanup single user: close all pipes, kill user's child process, kill user
 * xterm process, free-up slot.
 */
void cleanup_user(int idx, user_chat_box_t *users)
{
	close_pipes(idx, users);

	kill(users[idx].child_pid, SIGTERM);
	waitpid(users[idx].child_pid);
	kill(users[idx].pid, SIGTERM);

	users[idx].status = SLOT_EMPTY;
}

/*
 * Cleanup all users: given to you
 */
void cleanup_users(user_chat_box_t *users)
{
	int i;

	for (i = 0; i < MAX_USERS; i++) {
		if (users[i].status == SLOT_EMPTY)
			continue;
		cleanup_user(i, users);
	}
}

/*
 * Cleanup server process: close all pipes, kill the parent process and its
 * children.
 */
void cleanup_server(server_ctrl_t server_ctrl)
{
	/***** Insert YOUR code *******/
	close(server_ctrl.ptoc[1]);
	close(server_ctrl.ptoc[0]);
	close(server_ctrl.ctop[1]);
	close(server_ctrl.ctop[0]);

	kill(server_ctrl.child_pid, SIGTERM);
	waitpid(server_ctrl.child_pid);
	kill(server_ctrl.pid, SIGTERM);
}

/*
 * Utility function.
 * Find user index for given user name.
 */
int find_user_index(user_chat_box_t *users, char *name)
{
	int i, user_idx = -1;

	if (name == NULL) {
		fprintf(stderr, "NULL name passed.\n");
		return user_idx;
	}
	for (i = 0; i < MAX_USERS; i++) {
		if (users[i].status == SLOT_EMPTY)
			continue;
		if (strncmp(users[i].name, name, strlen(name)) == 0) {
			user_idx = i;
			break;
		}
	}

	return user_idx;
}

/*
 * Utility function.
 * Given a command's input buffer, extract name.
 */
char *extract_name(int cmd, char *buf)
{
	char *s = NULL;

	s = strtok(buf, " ");
	s = strtok(NULL, " ");
	if (cmd == P2P)
		return s;	/* s points to the name as no newline after name in P2P */
	s = strtok(s, "\n");	/* other commands have newline after name, so remove it*/
	return s;
}

/*
 * Send personal message. Print error on the user shell if user not found.
 */
void send_p2p_msg(int idx, user_chat_box_t *users, char *buf)
{

	char *name, *msg;
	char text[MSG_SIZE];
	const char *user_not_found = "User not found";
	int r_idx;

	if((name = extract_name(P2P, buf)) == NULL)
	{
		fprintf(stderr, "NULL name passed.\n");
		return;
	}
	msg = name;

	if((r_idx = find_user_index(users, name)) < 0) 
	{
		if (write(users[idx].ptoc[1], user_not_found, strlen(user_not_found) + 1) < 0)
			perror("write to child shell failed");
	}
	else
	{
		while(*msg != '\0'){
			msg++;
		}
		msg++;
		msg = strtok(msg, "\n");
		
		sprintf(text, "%s : %s", users[idx].name, msg);
		if (msg != NULL)
		{
			if (write(users[r_idx].ptoc[1], text, strlen(text) + 1) < 0)
				perror("write to child shell failed");
		}
	}

}

/*
 * Main:
 * Sets up pipes and execute shell for server.
 */
int main(int argc, char **argv)
{
	int i, p_c[2], c_p[2], flags, nread, status;
	char fd0[MSG_SIZE], fd1[MSG_SIZE];
	pid_t pid;

	/* open non-blocking bi-directional pipes for communication with server shell */
	if (pipe(p_c) == -1 || pipe(c_p) == -1)
	{
		fprintf(stderr, "Call to pipe error.\n");
		exit(1);
	}

	flags = fcntl(p_c[0], F_GETFL, 0);
	if (fcntl(p_c[0], F_SETFL, flags | O_NONBLOCK) == -1 )
	{
		fprintf(stderr, "Call to fcntl error.\n");
		exit(1);
	}

	flags = fcntl(c_p[0], F_GETFL, 0);
	if (fcntl(c_p[0], F_SETFL, flags | O_NONBLOCK) == -1 )
	{
		fprintf(stderr, "Call to fcntl error.\n");
		exit(1);
	}

	server_ctrl_t server;
	server.ptoc[0] = p_c[0];
	server.ptoc[1] = p_c[1];
	server.ctop[0] = c_p[0];
	server.ctop[1] = c_p[1];

	user_chat_box_t * users = malloc(MAX_USERS * sizeof *users);
	for( i = 0; i < MAX_USERS; i++)
	{
		users[i].status = SLOT_EMPTY;
	}

	pid = fork();

	if (pid == 0)
	{
		close(server.ptoc[1]);
		close(server.ctop[0]);

		sprintf(fd0, "%d", server.ptoc[0]);
		sprintf(fd1, "%d", server.ctop[1]);

		execl(strcat(strcat(CURR_DIR, "/"), SHELL_PROG), SHELL_PROG, fd0, fd1, "Server");
		fprintf(stderr, "Failed to execl.\n");
		exit(1);
	}
	else if (pid > 0)
	{
		close(server.ctop[1]);
		server.pid = getpid();

		pid_t user_pid;

		char buffer[MSG_SIZE];
		char user_fd0[MSG_SIZE], user_fd1[MSG_SIZE];
		char *name;
		int i, idx, retval, stat;

		while (1) {
			/* Let the CPU breathe */
			usleep(1000);

			/*
		 	 * 1. Read the message from server's shell, if any
		 	 * 2. Parse the command
		 	 * 3. Begin switch statement to identify command and take appropriate action
		 	 */
			nread = read(server.ctop[0], buffer, MSG_SIZE);
			if (nread == -1 ) {
				if (errno == EAGAIN) {
					fflush(stdout);
				} else {
					fprintf(stderr, "SERVER: Can't read from pipe.\n");
					exit(1);
				}
			} else if ( nread == 0 ) {
				printf("SERVER: End of conversation\n");
				fflush(stdout);
				exit(0);
			} else {
				switch(parse_command(buffer)) {
					case CHILD_PID:
						server.child_pid = atoi(extract_name(CHILD_PID, buffer));
						break;
					case LIST_USERS:
						list_users(users, server.ptoc[1]);
						break;
					case ADD_USER:
						name = extract_name(ADD_USER, buffer);
						if (name == NULL)
						{
							fprintf(stderr, "NULL name passed.\n");
						}
						else
						{
							if ((idx = add_user(users, name, server.ptoc[1])) >= 0)
							{
								user_pid = fork();
								if (user_pid == 0)
								{
									close(users[idx].ptoc[1]);
									close(users[idx].ctop[0]);

									sprintf(user_fd0, "%d", users[idx].ptoc[0]);
									sprintf(user_fd1, "%d", users[idx].ctop[1]);

									execl(XTERM_PATH, XTERM, "+hold", "-e", "./shell", user_fd0, user_fd1, name, (char*)NULL);
									fprintf(stderr, "Failed to execl.\n");
									exit(1);
								}
								else if (user_pid > 0)
								{
									close(users[idx].ctop[1]);
									users[idx].pid = user_pid;
								}
								else
								{
									fprintf(stderr, "failed to fork:\n");
									exit(1);
								}
							}
						}
						break;
					case KICK:
						name = extract_name(KICK, buffer);
						if ((idx = find_user_index(users, name)) >= 0)
						{
							cleanup_user(idx, users);
						}
						else
						{
							fprintf(stderr, "User %s does not exists.", name);
							// Prints a new promt to server
							if (write(server.ptoc[1], "", strlen("") + 1) < 0)
								perror("write to server shell failed");
						}
						break;
					case EXIT:
						cleanup_users(users);
						free(users);
						cleanup_server(server);						
						exit(0);
					case BROADCAST:
						broadcast_msg(users, buffer, server.ptoc[1], "Server");
						break;
					default:
						fprintf(stderr, "Invalid SERVER command used.");
						// Prints a new promt to server
						if (write(server.ptoc[1], "", strlen("") + 1) < 0)
							perror("write to server shell failed");
						break;
				}
				fflush(stdout);
			} // END IF

			/*
		 	 * 1. Read the message from user's shell, if any
		 	 * 2. Parse the command
		 	 * 3. Begin switch statement to identify command and take appropriate action
		 	 */
			for (i = 0; i < MAX_USERS; i++) {

				if (users[i].status == SLOT_EMPTY)
					continue;

				nread = read(users[i].ctop[0], buffer, MSG_SIZE);
				if (nread == -1 ) {
					if (errno == EAGAIN) {
						fflush(stdout);
					} else {
						fprintf(stderr, "USER: Can't read from pipe.\n");
					}
				} else if ( nread == 0 ) {
					waitpid (users[i].pid, &stat, WNOHANG);
					cleanup_user(i, users);
					fflush(stdout);
				} else {
					switch(parse_command(buffer)) {
						case CHILD_PID:
							users[i].child_pid = atoi(extract_name(CHILD_PID, buffer));
							break;
						case LIST_USERS:
							list_users(users, users[i].ptoc[1]);
							break;
						case P2P:
							send_p2p_msg(i, users, buffer);
							break;
						case EXIT:
							cleanup_user(i, users);
							break;
						case BROADCAST:
							broadcast_msg(users, buffer, server.ptoc[1], users[i].name);
							break;
						default:
							fprintf(stderr, "Invalid USER command from user %s.", users[i].name);
							// Prints a new promt to server
							if (write(server.ptoc[1], "", strlen("") + 1) < 0)
								perror("write to server shell failed");
							break;
					}
					fflush(stdout);
				}
			} // END FOR

		} // END WHILE
		wait(&status);
	}
	else
	{
		fprintf(stderr, "failed to fork:\n");
		exit(1);
	}

	return 0;
} // END MAIN

