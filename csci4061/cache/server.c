/*
*CSci4061 S2016 Assignment 4
*Group AN
*Name: Peter Nguyen, Yuanhao Lu, Phoebe Zhang, Rain Zhang
*X500: nguy1435,     luxx0267,   zhan2937,     zhan2223
*Date: 05/02/2016
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include "util.h"

#define MAX_THREADS 100
#define MAX_QUEUE_SIZE 100
#define MAX_REQUEST_LENGTH 64
#define MAX_CACHE_SIZE 100

static char *HTML = "text/html";
static char *JPEG = "image/jpeg";
static char *GIF = "image/gif";
static char *PLAIN = "text/plain";

static char WEB_DIR[MAX_REQUEST_LENGTH];

static pthread_mutex_t buffer_access = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t buffer_not_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t buffer_empty = PTHREAD_COND_INITIALIZER;

// Web server log file
FILE *log_file;

//Structure for queue.
typedef struct request_queue
{
        int     m_socket;
        char    m_szRequest[MAX_REQUEST_LENGTH];
} request_queue_t;

static request_queue_t buffer[MAX_QUEUE_SIZE];

static int in = 0;
static int out = 0;
static int count = 0;

int queue_length = 0;
int cache_size = 0;
static int cache_count = 0;

//Structure for cache
typedef struct cacheing_list
{
	char cache_request[MAX_REQUEST_LENGTH];
	char* cache_buffer;
	char* cache_content_type;
	int time;
	int size;
} cache_t;

static cache_t cache_array[MAX_CACHE_SIZE];

//Returns index of cache entry else -1 if not found
int find_in_cache(char* request){
	struct timeval tv;

	int i;
	for(i = 0; i < cache_size; i++) {
		if (strcmp(cache_array[i].cache_request, request) == 0) {
			gettimeofday(&tv, NULL);
			cache_array[i].time = (int) tv.tv_sec;
			return i;
		}
	}
	return -1;
	
}

//Add request to cache array
void add_to_cache(char* request, char* result_buffer, char* result_content_type, int fsize){
	struct timeval tv;

	//Cache is full need to remove oldest entry
	if (cache_count == cache_size) {
		int i, cache_delete_index = 0;
		for (i = 1; i < cache_size; i++) {
			if (cache_array[cache_delete_index].time > cache_array[i].time) {
				cache_delete_index = i;
			}
		}
		
		//Replace oldest entry with new request
		free(cache_array[cache_delete_index].cache_buffer);

		strcpy(cache_array[cache_delete_index].cache_request, request);
		cache_array[cache_delete_index].cache_buffer = malloc((fsize + 1) * sizeof(char));
		memcpy(cache_array[cache_delete_index].cache_buffer, result_buffer, fsize+1);
		cache_array[cache_delete_index].cache_content_type = result_content_type;
		gettimeofday(&tv, NULL);		
		cache_array[cache_delete_index].time = (int) tv.tv_sec;
		cache_array[cache_delete_index].size = fsize;
	}
	//Cache is not full, add entry
	else {
		strcpy(cache_array[cache_count].cache_request, request);
		cache_array[cache_count].cache_buffer = malloc((fsize + 1) * sizeof(char));
		memcpy(cache_array[cache_count].cache_buffer, result_buffer, fsize+1);
		cache_array[cache_count].cache_content_type = result_content_type;
		gettimeofday(&tv, NULL);		
		cache_array[cache_count].time = (int) tv.tv_sec;
		cache_array[cache_count].size = fsize;
		cache_count++;
	}
}

void * dispatch(void * arg)
{
	int item, i;
	char *filename = malloc(1024 * sizeof(char));
	
	while(1) {
		// accept a connection
                int fd;
		if ((fd = accept_connection()) < 0) {
			fprintf(stderr, "Connection error\n");
			pthread_exit(NULL);
		}

                // get the request
                if (get_request(fd, filename) == 0) {	
			pthread_mutex_lock(&buffer_access);
			while (count == queue_length)
				pthread_cond_wait(&buffer_empty, &buffer_access);
			
			buffer[in].m_socket = fd;
			strcpy(buffer[in].m_szRequest, WEB_DIR);
			strcat(buffer[in].m_szRequest, filename);
			in = (in + 1) % queue_length;
			count++;
			
			pthread_cond_signal(&buffer_not_empty);
			pthread_mutex_unlock(&buffer_access);
		}		
	}
        return NULL;
}

void * worker(void * arg)
{
	int item, i;
	int num = 1;
	request_queue_t request;
	struct timeval start_time;
	struct timeval end_time;
	int spent_time = 0;
	
	while(1) {	
		pthread_mutex_lock(&buffer_access);	
		
		while (count == 0)
			pthread_cond_wait(&buffer_not_empty, &buffer_access);
		
		gettimeofday(&start_time, NULL);	
		request = buffer[out];
		out = (out + 1) % queue_length;
		count--;

		int c_idx;
		if ((c_idx = find_in_cache(request.m_szRequest)) != -1) {
			//Cache hit
			return_result(request.m_socket, cache_array[c_idx].cache_content_type, cache_array[c_idx].cache_buffer, cache_array[c_idx].size);
			gettimeofday(&end_time, NULL);		
			spent_time = (int)end_time.tv_usec - (int)start_time.tv_usec;
			fprintf(log_file, "[%d][%d][%d][%s][%d][%dms][HIT]\n", *((int*)arg), num, request.m_socket, request.m_szRequest, cache_array[c_idx].size, spent_time);	
		}
		else {
			//Cache miss
			FILE *fp;
			if((fp = fopen(request.m_szRequest, "r")) != NULL) {
				fseek(fp, 0, SEEK_END);
				int fsize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
			
				char *buf = malloc(fsize + 1);
				char *result_content_type;
				fread(buf, fsize, 1, fp);

				if(strcmp(strrchr(request.m_szRequest, '.'), ".html") == 0) {
					return_result(request.m_socket, HTML, buf, fsize);
					result_content_type = HTML;
				}
				else if (strcmp(strrchr(request.m_szRequest, '.'), ".jpg") == 0) {
					return_result(request.m_socket, JPEG, buf, fsize);
					result_content_type = JPEG;
				}
				else if (strcmp(strrchr(request.m_szRequest, '.'), ".gif") == 0) {
					return_result(request.m_socket, GIF, buf, fsize);
					result_content_type = GIF;
				}
				else {
					return_result(request.m_socket, PLAIN, buf, fsize);
					result_content_type = PLAIN;
				}

				if (cache_size > 0){
					add_to_cache(request.m_szRequest, buf, result_content_type, fsize);
				}

				fclose(fp);
				free(buf);
				gettimeofday(&end_time, NULL);
				spent_time = (int)end_time.tv_usec - (int)start_time.tv_usec;
				fprintf(log_file, "[%d][%d][%d][%s][%d][%dms][MISS]\n", *((int*)arg), num, request.m_socket, request.m_szRequest, fsize, spent_time);
			}
			else {
				char *error = malloc(18);
				strcpy(error, "File not found");
				return_error(request.m_socket, error);
				free(error);
				gettimeofday(&end_time, NULL);
				spent_time = (int)end_time.tv_usec - (int)start_time.tv_usec;
				fprintf(log_file, "[%d][%d][%d][%s][File not found.][%dms][MISS]\n", *((int*)arg), 1, request.m_socket, request.m_szRequest, spent_time);
			}
		}

		num++;
		fflush(log_file);
		if (count == 0 )
			pthread_cond_signal(&buffer_empty);

		pthread_mutex_unlock(&buffer_access);	
	}
        return NULL;
}

//^C, close the program and file
void handler(int sigid) {
        fprintf(stderr, "quit!\n");
        fclose(log_file);
        exit(0);
}

int main(int argc, char **argv)
{
        //Error check first.
        if(argc != 6 && argc != 7)
        {
                printf("usage: %s port path num_dispatcher num_workers queue_length [cache_size]\n", argv[0]);
                return -1;
        }

        //set signal handler for ctrl^C so that we can correctly close the log file before exiting
        struct sigaction act;
        act.sa_handler = handler;
        sigaction(SIGINT, &act, NULL);

        int port = atoi(argv[1]);
	init(port);

	int dispatch_nums, worker_nums;

	//open the log file for writing
        log_file = fopen("web_server_log", "w");
        if (log_file == NULL) {
                fprintf(stderr, "Failed to open log file\n");
                return -1;
        }

	//Save web root directory
	strcpy(WEB_DIR, argv[2]);

        dispatch_nums = atoi(argv[3]);
        if (dispatch_nums <= 0) {
		dispatch_nums = 1;
	}
	else if (dispatch_nums > MAX_THREADS) {
		dispatch_nums = MAX_THREADS;
        }
        
        worker_nums = atoi(argv[4]);
        if (worker_nums <= 0) {
		worker_nums = 1;
	}
	else if (worker_nums > MAX_THREADS) {
		worker_nums = MAX_THREADS;
        }
        
        queue_length = atoi(argv[5]);
        if (queue_length <= 0 ) {
		queue_length = 1;
	}
	else if (queue_length > MAX_QUEUE_SIZE) {
		queue_length = MAX_QUEUE_SIZE;
        }

	if (argc == 7) {
		cache_size = atoi(argv[6]);
		if (cache_size <= 0 ) {
			cache_size = 0;
		}
		else if (cache_size > MAX_CACHE_SIZE) {
			cache_size = MAX_CACHE_SIZE;
        	}
	}

        int result, i;
	int args_d[dispatch_nums];
	int args_w[worker_nums];
	pthread_t dispatcher_t[dispatch_nums];
	pthread_t worker_t[worker_nums];

        // Create threads
        for (i = 0; i < worker_nums; i++) {
		args_w[i] = i;	
		if(pthread_create(&worker_t[i], NULL, worker, (void*)&args_w[i]) != 0) {
			fprintf(stderr, "Error creating worker thread\n");
			exit(1);
		}			
        }

        for (i = 0; i < dispatch_nums; i++) {
		args_d[i] = i;
            	if(pthread_create(&dispatcher_t[i], NULL, dispatch, (void*)&args_d[i]) != 0) {
			fprintf(stderr, "Error creating dispatcher thread\n");
			exit(1);
		}
        }

	// Join threads
        for (i = 0; i < dispatch_nums; i++) {
            if(pthread_join(dispatcher_t[i], NULL) != 0)
		fprintf(stderr, "Error joining dispatcher thread\n");
        }

	for (i = 0; i < worker_nums; i++) {
            if(pthread_join(worker_t[i], NULL) != 0)
		fprintf(stderr, "Error joining worker thread\n");
	}

	//close the file
        fclose(log_file);

        return 0;
}

