#ifndef __UTILS_H_
#define __UTILS_H_

#include <sys/time.h>
#include "config.h"

typedef enum
{
    ANS_OK,
    ANS_VIOLATE,
    ANS_CRASH,
} exit_status_t;

extern struct timeval start_time;

/*
 * Initialize utils, e.g. set start_time
 */
void
init_utils(int argc, char *argv[]);

/*
 * Error, Log, Debug
 */
void
step(const char *fmt, ...);

void
error(int exit_code, const char *fmt, ...);

void
serror(int exit_code, const char *fmt, ...);

#ifdef DEBUG
#define debug(...) step(__VA_ARGS__)
#else 
#define debug(...) do { } while (0)
#endif

/*
 * current_time - start_time
 */
double
timedelta(void);

/*
 * Statuses for hoster
 */
void
status(exit_status_t status, const char *fmt, ...);

#endif

