#ifndef __STDIO_EXTPROC__
#define __STDIO_EXTPROC__



#include "version_config.hpp"

#ifdef _WIN32

#else
// see https://tldp.org/LDP/lpg/node11.html
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#endif // _WIN32

// cross platform data structure for external process execution
struct ExtProcStruct
{
    char* inCmd;
    char* inArgs[5];
    int stdinfds[2];
    int stdoutfds[2];
    int stderrfds[2];
    int errnoPipe;
    int errnoFork;
    int errnoDup;
    int errnoExec;
    pid_t childpid;
};

struct ExtProcStruct createExtProcStruct(const char* Cmd, const char* Args[]);

// main entry point of the server/parent process
// - creates pipes and forks proc running ext application
// - duplicates stdio on created and forked pipes
// -
void ExtProcMainThread(struct ExtProcStruct* extStructPtrIn);


#ifdef __cplusplus

class stdioExtProcClass
{
    // could be launched from an application either synchronously or asynchrounsously
    // creates pipes to be linked to stdIO of  ext proc
    // forks child process with copies of pipe file descriptors
    // use dup or dup2 to duplicate fds on stdin, stdout, stderr



};

#endif

#endif // __STDIO_EXTPROC__
