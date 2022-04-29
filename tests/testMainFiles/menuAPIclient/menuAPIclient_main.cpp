#include "menuAPIclient.hpp"
#include "Platform_ccOS.hpp"

static bool keepGoing = true;
static char keysInput[charBuffMax] = "";
static TCPmenuAPIClientclass Console_UI((char*)"127.0.0.1", (uint16_t)55500, &keysInput[0]);

int main(int argc, char* argv[])
{    
    while (keepGoing)
    {
        try
        {
            if (argc > 1)
            {
                char* ulString = argv[1];
                if (stringMatchCaseSensitive(ulString, "Operator"))
                    Console_UI.UserLevelString = "Operator";
                else if(stringMatchCaseSensitive(ulString, "Administrator"))
                    Console_UI.UserLevelString = "Administrator";
                else if(stringMatchCaseSensitive(ulString, "Developer"))
                    Console_UI.UserLevelString = "Developer";
                else
                    Console_UI.UserLevelString = "Observer";
            }
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