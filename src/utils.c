#include "utils.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct timeval start_time;
static char status_file[512];

void
init_utils(int argc, char *argv[])
{
    gettimeofday(&start_time, 0);
    strcpy(status_file, STATUS_FILE);
}

double
timedelta(void)
{
    struct timeval current;
    gettimeofday(&current, 0);

    return (current.tv_sec  - start_time.tv_sec)
         + (current.tv_usec - start_time.tv_usec) / 1000000.0;
}

void
step(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    fprintf(stderr, "[%03.4lf,%4d] ", timedelta(), getpid());
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");

    va_end(ap);
}

void
error(int exit_code, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    fprintf(stderr, "[%03.4lf,%4d] ", timedelta(), getpid());
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");

    va_end(ap);

    if (exit_code)
    {
        status(ANS_CRASH, "Internal error");
        exit(exit_code);
    }
}

void
serror(int exit_code, const char *fmt, ...)
{
    va_list ap;
    double time;

    va_start(ap, fmt);
    time = timedelta();

    fprintf(stderr, "[%03.4lf,%4d] SYSERROR: errno = %d ", time, getpid(), errno);
    perror("DESC");

    fprintf(stderr, "[%03.4lf,%4d] ", timedelta(), getpid());
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");

    va_end(ap);

    if (exit_code)
    {
        status(ANS_CRASH, "Internal error");
        exit(exit_code);
    }
}

void
status(exit_status_t s, const char *fmt, ...)
{
    FILE *f;
    char *status;
    va_list ap;

    va_start(ap, fmt);

    if (NULL == (f = fopen(status_file, "w")))
    {
        serror(0, "mmm... Cant create status file '%s'", status_file);
        exit(1);
        /* NOTREACHED */
    }

    switch (s)
    {
    case ANS_OK:
        status = "ok";
        break;
    case ANS_VIOLATE:
        status = "violate";
        break;
    case ANS_CRASH:
        status = "crash";
        break;
    default:
        status = "unknown";
        break;
    }

    fprintf(f, "{\"status\":\"%s\",\"comment\":\"", status);
    vfprintf(f, fmt, ap);
    fprintf(f, "\"}");
    fclose(f);

    va_end(ap);
}

