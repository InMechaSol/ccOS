#ifndef __OSEXESYS__
#define __OSEXESYS__

#include "version_config.hpp"
#include "execution_system.h"
//#include "console_menu.h"
#include <vector>
#include <thread>
#include <climits>

#define uSEC_PER_CLOCK (1000000/CLOCKS_PER_SEC)

class exe_thread_class;

class OSexecutionSystemClass : public executionSystemClass
{
protected:
    bool allThreadsSetup = false;
    bool allThreadsRunning = false;
public:
    std::vector<exe_thread_class*> exeThreadModuleList;
    std::vector<std::thread*> exeThreadList;
    OSexecutionSystemClass(
        UI_32 uSperTick
    );
    void ExecuteSetup();
    void ExecuteLoop();
    void ExecuteSysTick();

};

class ccOSApplicationClass
{
public:
    virtual void LinkAndStartExeThreads() = 0;
};

#endif //!__OSEXESYS__
