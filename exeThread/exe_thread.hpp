#ifndef __EXETHREAD__
#define __EXETHREAD__

#include "compute_module.h"
#include "version_config.hpp"
#include <cstring>
#include <thread>

class exe_thread_class : public computeModuleClass
{
private:
    bool setupComplete = false;
    bool permitMainLoop = false;
    bool isRunning = false;

protected:
    uint32_t ErrorBits = 0;
    uint32_t WarnBits = 0;
    int mainLoopMS = 1000;

public:
    exe_thread_class(struct computeModuleStruct* compModDataPtrIn);
    virtual char* IDString() = 0;
    bool getSetupComplete();
    uint32_t getErrorBits();
    uint32_t getWarnBits();
    virtual void setMainLoopMS(int msIN);
    virtual int getMainLoopMS();
    void resetIsRunning();
    bool getIsRunning();
    void enableMainLoop();
    void ThreadExecute();    
};

#endif // !__EXETHREAD__