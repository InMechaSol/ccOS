#include "os_execution_system.hpp"
#include "exe_thread.hpp"


// 1) Platform Setup Function
void platformSetup()
{
    //<platformSetup>
    // 
    // open config device
    configFile.open("conFile.json");
    LogFile.open("logFile.json");
    // read config string?? 
    // 
    // open log device
    // wrtie log string??
    // 
    //</platformSetup>
}
// 2) Platform Start Function
void platformStart()
{
    //<platformStart>
    //</platformStart>
}
// 3) Platform Loop Delay Function
void platformLoopDelay()
{
    //<platformLoopDelay>
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //</platformLoopDelay>
}



// 8) Platform API Functions (From Template?)
PlatformAPIFuncsTemplate(size + 1);


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