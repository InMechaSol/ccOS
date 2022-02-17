#include "exe_thread.hpp"
#include <iostream>

exe_thread_class::exe_thread_class(struct computeModuleStruct* compModDataPtrIn) : computeModuleClass(compModDataPtrIn)
{

}

bool exe_thread_class::getSetupComplete()
{
    return setupComplete;
}

void exe_thread_class::resetIsRunning()
{
    isRunning = false;
}
bool exe_thread_class::getIsRunning()
{
    return isRunning;
}
void exe_thread_class::enableMainLoop()
{
    permitMainLoop = true;
}
uint32_t exe_thread_class::getErrorBits()
{
    return ErrorBits;
}

uint32_t exe_thread_class::getWarnBits()
{
    return WarnBits;
}

void exe_thread_class::setMainLoopMS(int msIN)
{
    if (msIN >= 0)
        mainLoopMS = msIN;
}
int exe_thread_class::getMainLoopMS()
{
    return mainLoopMS;
}

void exe_thread_class::ThreadExecute()
{
    try
    {
        mod_setup();
        setupComplete = true;
    }
    catch (std::exception& e)
    {
        // error handling
        std::cout << "Standard exception: " << e.what() << std::endl;
    }
    catch (std::string& e)
    {
        std::cout << e + "\n";
    }
    catch (char*& e)
    {
        std::cout << *e << std::endl;
    }
    catch (...)
    {

    }
    for (;;)
    {
        try
        {
            if (mainLoopMS > 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(mainLoopMS));
            else
                std::this_thread::yield();

            if (permitMainLoop)
            {
                mod_loop();
                isRunning = true;
            }

        }
        catch (std::exception& e)
        {
            // cyclic error handling
            std::cout << "Standard exception: " << e.what() << std::endl;
        }
        catch (std::string& e)
        {
            std::cout << e + "\n";
        }
        catch (char*& e)
        {
            std::cout << *e << std::endl;
        }
        catch (...)
        {

        }
    }
}
