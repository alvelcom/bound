#define _GNU_SOURCE

#include <bsd/unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#include "check.h"
#include "utils.h"

static int
syscallwall(pid_t pid, int *status, config_t *config, result_t *result);

static int
run_standalone(config_t *config, result_t *result)
{
    pid_t pid;
    struct rlimit new;

    if (0 != (pid = fork()))
    {
        /* parent */
        /* wait for child */
        int status;
        debug("I'm parent. I'm waiting for a child");
        if (-1 == waitpid(pid, &status, 0))
        {
            /* FIXME */
            serror(0, "bad waitpid");
            kill(pid, SIGKILL);
            waitpid(pid, &status, 0);
            error(1, "exit after bad waitpid");

            /* NOTREACHED */
        }

        debug("I've got");

        if (-1 == ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD))
        {
            /* FIXME */
            serror(0, "bad ptrace");
            kill(pid, SIGKILL);
            waitpid(pid, &status, 0);
            error(1, "exit after bad ptrace");

            /* NOTREACHED */
        }

        debug("Starting syscall wall");

        if (syscallwall(pid, &status, config, result))
            error(0, "syscallwall goes wrong");

        /*
         * At this point child process should be already died
         */

    }
    else
    {
        /* child */
        step("Starting %s", config->argv[0]);
        setproctitle("i'm patient");

        new.rlim_cur = new.rlim_max = config->cpu_limit / 1000 + 2;
        if (setrlimit(RLIMIT_CPU, &new))
            serror(0, "bad setrlimit (cpu)");
        
        new.rlim_cur = new.rlim_max = config->memory_limit;
        if (setrlimit(RLIMIT_DATA, &new))
            serror(0, "bad setrlimit (data)");

        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execvp(config->argv[0], config->argv);

        /* NOTREACHED */
    }
    return 0;
}

int
syscallwall(pid_t pid, int *status, config_t *config, result_t *result)
{
    struct user_regs_struct regs;
    struct rusage ru;

    while (!WIFEXITED(*status) || !WIFSIGNALED(*status))
    {
        if (-1 == ptrace(PTRACE_SYSCALL, pid, 0, 0))
        {
            serror(0, "bad ptrace at %d", __LINE__);
            break;
        }
        wait4(pid, status, 0, &ru);

        if (WIFEXITED(*status) || WIFSIGNALED(*status))
            break;

        if (!WIFSTOPPED(*status) || !(WSTOPSIG(*status) & 0x80))
            continue;

        /* syscall here */
        ptrace(PTRACE_GETREGS, pid, 0, &regs);
        debug("SYSCALL %d at %08lx", regs.orig_rax, regs.rip);

        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        wait4(pid, status, 0, &ru);
        debug("/SYSCALL");
    }

    if (WIFSIGNALED(*status))
    {
        step("Patient terminated by system");
        result->terminated = 1;
    }
    else
        result->terminated = 0;

    result->cpu_time = ru.ru_utime.tv_sec + ru.ru_utime.tv_usec / 1000000.0;
    result->memory   = ru.ru_maxrss;

    return 0;
}

int
run(runtype_t type, config_t *config, result_t *result)
{
    if (RUN_STANDALONE == type)
        return run_standalone(config, result);

    error(2, "bad run type");
    /* NOT REACHED */

    return -1;
}
