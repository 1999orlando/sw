#ifndef FORMAT_H
#define FORMAT_H

#include <sys/time.h>

#define ASCIINUMBER(c) (c >= '0' && c <= '9')

/* States for */
typedef enum {
	NOTVALID,
	MILLISECONDS,
	SECONDS,
	DECISECONDS,
	CENTISECONDS,
	MICROSECONDS
} time_unit_t;


void timeval_formatstr(struct timeval tv, char *str);
long str_formatusec(char *str);

#endif
