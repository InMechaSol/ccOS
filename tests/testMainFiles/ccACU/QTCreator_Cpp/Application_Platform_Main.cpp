#include "Application_Platform_Main.hpp"


ccACU_ApplicationClass theApplicationExample(&theExecutionSystem);

//ccOS_MAIN_TEMPLATE

ccOSVersionsTemplate
int main(int argc, char** argv)
{
    // Start the systick thread
    std::thread systickThread(&OSexecutionSystemClass::ExecuteSysTick, std::ref(theExecutionSystem));

    // Start the exe_thread modules
    theApplicationExample.LinkAndStartExeThreads();

    // run setup
    theExecutionSystem.ExecuteSetup();
    for (;;)
    {
        // run loop
        theExecutionSystem.ExecuteLoop();
    }
    return RETURN_ERROR;
}
