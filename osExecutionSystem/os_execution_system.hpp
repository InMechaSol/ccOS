#ifndef __OSEXESYS__
#define __OSEXESYS__

#include "version_config.hpp"
////////////////////////////////
// Bring In ccNOos
#include "execution_system.h"

#include <ctime>
#include <thread>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <fstream>

#include <vector>
#include <thread>

#define LIGHT_OFF (0u)      // 1-PSoC4, 0-most others
#define uSEC_PER_CLOCK (1000000/CLOCKS_PER_SEC)

// 0) (Optional) Platform Config and Log Files/Devices
std::ifstream configFile;
std::ofstream LogFile;






#define __ccOS_MAIN_TEMPLATE ccOSVersionsTemplate \
int main(int argc, char** argv)\
{\
    clock_t tlast = clock();\
    clock_t tnow, tdelta;\
    UI_32* uSecTicksPtr = &exeSystem.getExeDataPtr()->uSecTicks;\
    UI_32* hourTicksPtr = &exeSystem.getExeDataPtr()->hourTicks;\
    exeSystem.ExecuteSetup();\
    for (;;)\
    {\
        tnow = clock();\
        if (tnow >= tlast)\
            tdelta = tnow - tlast;\
        else\
            tdelta = tnow + (LONG_MAX - tlast);\
        tlast = tnow;\
        (*uSecTicksPtr) += tdelta * uSEC_PER_CLOCK;\
        if ((*uSecTicksPtr) >= TIME_uS_PER_HR)\
        {\
            (*uSecTicksPtr) = 0u;\
            (*hourTicksPtr)++;\
        }\
        exeSystem.ExecuteLoop();\
    }\
    return RETURN_ERROR;\
}
#define ccOS_MAIN_TEMPLATE __ccOS_MAIN_TEMPLATE


class OSexecutionSystemClass : public executionSystemClass
{
private:
    std::vector<exe_thread_class*> exeThreadModuleList;
    std::vector<std::thread> exeThreadList;

public:
    OSexecutionSystemClass(
        UI_32 uSperTick
    );
    void ExecuteSetup();
    void ExecuteLoop();
    void ExecuteSysTick();
};

#endif //!__OSEXESYS__