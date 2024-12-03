#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include <errno.h>


#include "sw.h"
#include "format.h"
#include "termios_helper.h"

#define DEFAULT_PRECISION 32768

void print_usage(FILE *stream, char *basename){
	fprintf(stream, "Usage: %s [-p precision in microseconds] [-s how many entries in the stops table]", basename);
	fprintf(stream, "\n\t while running use [s] or [S] to stop/start the stopwatch");
	fprintf(stream, "\n\t use [r] or [R] to reset the stopwatch");
	fprintf(stream, "\n\t use [space] to record time");
	fprintf(stream, "\n\t use [q] to quit\n");
}

int main(int argc, char *argv[]){

	struct stopwatch sw;

	struct termios original_settings;

	int opt;

	char c = 0;

	long default_precision = DEFAULT_PRECISION;
	long default_number_of_stops = 16;
	int bufsize = 8;


	while((opt = getopt(argc, argv, "p:s:h")) != -1 ){
		switch(opt){
			case 'p':
				// this string
//				default_precision = atol(optarg);
				default_precision = str_formatusec(optarg);
				if ( default_precision < 0 ){
					fprintf(stderr, "please select a valid range of microseconds\n");
					exit(EXIT_FAILURE);
					}
				break;
			case 's':
				default_number_of_stops = atoi(optarg);
				if ( default_number_of_stops < 0 || default_number_of_stops > 64 ){
					fprintf(stderr, "please select a valid range of stops\n");
					exit(EXIT_FAILURE);
					}
				break;
			case '?':
				print_usage(stderr, argv[0]);
				exit(EXIT_FAILURE);
				break;
			case 'h':
				print_usage(stdout, argv[0]);
				exit(EXIT_SUCCESS);
				break;

		}
	}


	termios_init(&original_settings);
	on_exit(on_exit_termios_restore, (void *)&original_settings);

	// set this using getopt
	sw.precision=default_precision;
	sw.nstops=default_number_of_stops;

	sw_init(&sw);

	char *example_buffer = (char *)malloc(sizeof(char) * bufsize);
	setvbuf(stdout, example_buffer, _IOFBF, bufsize);
//	free(example_buffer);

	sw_run(&sw);

	// exit
	exit(EXIT_SUCCESS);
}
