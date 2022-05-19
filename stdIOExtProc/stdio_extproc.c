#include "stdio_extproc.h"

struct ExtProcStruct createExtProcStruct(const char* Cmd, const char* Args[])
{
    struct ExtProcStruct outStruct;
    outStruct.inCmd = (char*)Cmd;
    for(int i = 0; i<5; i++)
        outStruct.inArgs[i] = (char*)Args[i];
}

#ifdef _WIN32
void ExtProcMainThread(struct ExtProcStruct* extStructPtrIn)
{
}
#else
// main entry point of the server/parent process
// - creates pipes and forks proc running ext application
// - duplicates stdio on created and forked pipes
// -
void ExtProcMainThread(struct ExtProcStruct* extStructPtrIn)
{
    // create three pipes (stdin, stdout, stderr)
    if(pipe(extStructPtrIn->stdinfds)==-1)
    {
        perror("pipe0");
        return;
    }
    if(pipe(extStructPtrIn->stdoutfds)==-1)
    {
        perror("pipe1");
        return;
    }
    if(pipe(extStructPtrIn->stderrfds)==-1)
    {
        perror("pipe2");
        return;
    }

    // fork a child process to run external program
    if((extStructPtrIn->childpid = fork()) == -1)
    {
        perror("fork");
        return;
    }



    // If running in the child process
    if(extStructPtrIn->childpid == 0)
    {
        /* Close stdin, duplicate the input side of pipe to stdin */
        dup2(0, extStructPtrIn->stdinfds[0]);
        /* Close stdout, duplicate the output side of pipe to stdout */
        dup2(1, extStructPtrIn->stdoutfds[1]);
        /* Close stderr, duplicate the output side of pipe to stderr */
        dup2(2, extStructPtrIn->stderrfds[1]);

        // execute external process
        execv(extStructPtrIn->inCmd, extStructPtrIn->inArgs);

        close(extStructPtrIn->stdinfds[1]);
        close(extStructPtrIn->stdoutfds[0]);
        close(extStructPtrIn->stderrfds[0]);

        // exit child process
        exit(0);
    }
    // If running in the parent process
    else
    {
        /* Parent process closes up output side of pipe */
        close(extStructPtrIn->stdinfds[1]);
        /* Parent process closes up input side of pipe */
        close(extStructPtrIn->stdoutfds[0]);
        /* Parent process closes up input side of pipe */
        close(extStructPtrIn->stderrfds[0]);

        // loop reading stdin/stderr, wait for close of child process
        UI_8 stopLoop = ui8FALSE;
        pid_t childPid = 0;
        int status = 0;
        do
        {
            // write to pipe if desired?

            // allow other threads to execute
            usleep(10000);  // 10 ms

            // non blocking read of pipes

            // check for application close
            childPid = waitpid(-1,&status,WNOHANG);
            if(childPid!=0)
                stopLoop = ui8TRUE;

        }while(stopLoop==ui8FALSE);

        close(extStructPtrIn->stdinfds[0]);
        close(extStructPtrIn->stdoutfds[1]);
        close(extStructPtrIn->stderrfds[1]);

    }

    return;
}

#endif // _WIN32 or else
