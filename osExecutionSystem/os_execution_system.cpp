#include "os_execution_system.hpp"
#include "exe_thread.hpp"





#ifdef __REALTIME_SYSTICK__
OSexecutionSystemClass::OSexecutionSystemClass(UI_32 uSperTick) :executionSystemClass(uSperTick) { ; }
#else 
OSexecutionSystemClass::OSexecutionSystemClass(UI_32 uSperTick) : executionSystemClass(0) { ; }
#endif

void OSexecutionSystemClass::ExecuteSetup()
{
	executionSystemClass::ExecuteSetup();

	// and managed exe threads
}

void OSexecutionSystemClass::ExecuteLoop()
{
	executionSystemClass::ExecuteLoop();

	// and managed exe threads
    if (!allThreadsSetup)
    {
        for (unsigned int i = 0; i < exeThreadModuleList.size(); i++)
        {
            if (!exeThreadModuleList[i]->getSetupComplete())
                break;
            if (i == exeThreadModuleList.size()-1)
            {
                allThreadsSetup = true;
                for (unsigned int j = 0; j < exeThreadModuleList.size(); j++)
                    exeThreadModuleList[j]->enableMainLoop();                
            }
        }
    }
    else
    {
        allThreadsRunning = true;
        for (unsigned int i = 0; i < exeThreadModuleList.size(); i++)
        {
            if (!exeThreadModuleList[i]->getIsRunning())
            {
                allThreadsRunning = false;
            }
            exeThreadModuleList[i]->resetIsRunning();
        }
    }
}

// The "systick" entry point
void OSexecutionSystemClass::ExecuteSysTick()
{// gotta hard real-time systick entry point, called cyclically via ISR
#ifdef __REALTIME_SYSTICK__
    data.uSecTicks += data.uSecPerSysTick;
    if (data.uSecTicks >= TIME_uS_PER_HR)
    {
        data.uSecTicks = 0u;
        data.hourTicks++;
    }
    // module systick execution area
    ModuleExeArea(EXP_SYSTICK);
    
#else // gotta non-real-time systick entry point, run in a thread
    clock_t tlast = clock();
    clock_t tnow, tdelta;      
    for (;;)
    {
        tnow = clock();
        if (tnow >= tlast)
            tdelta = tnow - tlast;
        else
            tdelta = tnow + (LONG_MAX - tlast);
        tlast = tnow;
        data.uSecTicks += tdelta * uSEC_PER_CLOCK;
        if (data.uSecTicks >= TIME_uS_PER_HR)
        {
            data.uSecTicks = 0u;
            data.hourTicks++;
        }
        // module systick execution area
        ModuleExeArea(EXP_SYSTICK);
        platformLoopDelay();
    }
#endif   
}