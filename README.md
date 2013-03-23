bound
=====

This program demonstrates how to create a basic sandbox for solution checking.

We consider that solution is some executable file (only `./under` at this point).

This project in the early stage of development. That means you _really_ should not use
it in production.

That are performed when you're doing `./bound`, let's
describe sequence of them:
* in `run_standalone` process forks;
* then in child process we set up limits for cpu and memory;
* after that child process says TRACEME;
* meanwhile parent process waits for this in `waitpid`;
* child process performing `execv`, that changes image to `./under`;
* `./under` is executed;
* meanwhile parent waits for a syscall in child process or just for child termination;
* the situation when syscall is taking place in child is caught by parent in the
  `syscallwall`, `syscallwall` just print debug message at this point.

Short todo
----------

- [x] Create syscall hooker
- [x] Set up memory and cpu limit
- [ ] Port it to x86 Linux (currently only Linux amd64 is supported). Should be easy.
- [ ] Port it to Windows (needs complete rewrite of check.c)
- [ ] Port it to FreeBSD (should be not too hard)
- [ ] Create normal interaction specification
- [ ] Make It customizable
- [ ] Create syscall filter
- [ ] Create test framework
- [ ] Decide about another execution model (not only standalone)
- [ ] Continue this todo...

Developer installation advices
------------------------------

Currently this project is able to run only on linux with amd64 arch. That is you need
linux with amd64 architecture, at least on virtual machine.

If you decide to develop on debian-family linuxes I can guess which packages you
should have on your system:

```
$ sudo apt-get install make gcc build-essential git
```

Now you can fetch the git repo:

```
$ git clone http://github.com/alvelcom/bound.git
$ cd bound
```

Now you are ready to build:

```
$ make
```

If you could see `bound` file then build was successful. Now you need to cook 
the payload or `under`. You could compile `test/t02.c`:

```
$ gcc -o under test/t02.c
```

`t02` is forever loop test, you could check it (use Ctrl-C to break):

```
$ ./under
```

Now you could run `bound` which should automatically terminate under after
about 3 second.

```
$ ./bound
```

What to read
------------

At first you should understand what is syscalls and why we should keep they under
control. 

* [ptrace at habrahabr](http://habrahabr.ru/post/111266/)
* man fork(2): 
  [linux](http://linux.die.net/man/2/fork), 
  [freebsd](http://www.freebsd.org/cgi/man.cgi?query=fork)
* man waitpid(2):
  [linux](http://linux.die.net/man/2/waitpid),
  [freebsd](http://www.freebsd.org/cgi/man.cgi?query=waitpid)
* man ptrace(2):
  [linux](http://linux.die.net/man/2/ptrace),
  [freebsd](http://www.freebsd.org/cgi/man.cgi?query=ptrace)
* man setrlimit(2):
  [linux](http://linux.die.net/man/2/setrlimit),
  [freebsd](http://www.freebsd.org/cgi/man.cgi?query=setrlimit)


