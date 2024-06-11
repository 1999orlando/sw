#include "sw.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

void sw_init(struct stopwatch *sw){
	gettimeofday(&sw->tv, NULL);

	memset(&sw->acc, 0, sizeof(sw->acc));
	memset(&sw->stops, 0, sizeof(struct timeval) * sw->nstops);
}

void sw_update(struct stopwatch *sw){
	gettimeofday(&sw->running_tv, NULL);

	sw->acc.tv_sec = sw->running_tv.tv_sec - sw->tv.tv_sec;
	sw->acc.tv_usec = sw->running_tv.tv_usec - sw->tv.tv_usec;
}

void sw_reset(struct stopwatch *sw){
	gettimeofday(&sw->tv, NULL);

	memset(&sw->acc, 0, sizeof(sw->acc));
}


void sw_rectime(struct stopwatch *sw){
	memmove(sw->stops + 1, sw->stops, sizeof(struct timeval) * (sw->nstops - 1));

	sw->stops[0].tv_sec = sw->acc.tv_sec;
	sw->stops[0].tv_usec = sw->acc.tv_usec;
}


void sw_show(struct stopwatch *sw){
	/* clear terminal screen and move cursor on top-left corner */
	printf("\033[2J\033[H");

	char buffer[16];
	for (int i=0; i < sw->nstops; i++){
		timeval_formatstr(sw->stops[i], buffer);
		printf("\ttime no. %02d: %10s\n", i+1, buffer);
	}
	printf("\n");
}

void sw_stop(struct stopwatch *sw){
	sw->running = SWITCH(sw->running);
	gettimeofday(&sw->tv, NULL);
	sw->tv.tv_sec -= sw->acc.tv_sec;
	sw->tv.tv_usec -= sw->acc.tv_usec > 0 ? sw->acc.tv_usec : -sw->acc.tv_usec;
}

void sw_run(struct stopwatch *sw){

	char c = 0;
	char buffer[16];

	sw->running=1;
	timeval_formatstr(sw->acc, buffer);
	printf("\r\t> %s", buffer);

	for(;;){
		if (read(STDIN_FILENO, &c, 1))
			switch(c){
				case 'R':
				case 'r':
					sw_reset(sw);
					break;
				case 'S':
				case 's':
					sw_stop(sw);
					break;
				case ' ':
					sw_rectime(sw);
					sw_show(sw);
					break;
				case 'q':
				case 'Q':
					exit(EXIT_SUCCESS);
					break;
			}

		if (sw->running)
			sw_update(sw);

		timeval_formatstr(sw->acc, buffer);
		printf("\r\t> %s", buffer);

		usleep(sw->precision);
	}
}
