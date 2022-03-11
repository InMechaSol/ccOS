#include "menuAPIclient.hpp"
#include "Platform_ccOS.h"

static bool keepGoing = true;
static char keysInput[charBuffMax] = "";
static TCPmenuAPIClientclass Console_UI((char*)"127.0.0.1", (uint16_t)55500, &keysInput[0]);

int main(int argc, char* argv[])
{    
    while (keepGoing)
    {
        try
        {
            std::cout << "Got this many Args: " << argc-1 << std::endl << std::flush;
            if (argc > 1)
                std::cout << "Got this  Argv: " << argv[1] << std::endl << std::flush;
            std::cout << "Press 'd' then Enter to Continue" << std::flush;
            std::cin >> keysInput;
            Console_UI.ThreadExecute();
        }
        catch (std::exception& e)
        {
            // cyclic error handling
            std::cout << "Standard exception: " << e.what() << std::endl;
            std::cin >> keysInput;
        }
        catch (...)
        {
            std::cout << "Un-Known exception: " << std::endl;
            std::cin >> keysInput;
        }
    }

}