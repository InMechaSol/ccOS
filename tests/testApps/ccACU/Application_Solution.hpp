#ifndef __APP_SOLUTION__
#define __APP_SOLUTION__

// include the SatComACS Application and Serialization from ccNOos Layer
#include "PlatformApp_Serialization.h"
// include ccOS execution system and exe thread module
#include "os_execution_system.hpp"
#include "exe_thread.hpp"
// include ccOS comm modules and libraries
#include "serial_comms.hpp"
#include "udp_comms.hpp"
#include "tcp_comms.hpp"
#include "stdio_extproc.hpp"

////////////////////////////////////////////////////////////////////////////////
// C++ ccOSTests Example Application - built from ccOS components
#define __ccOS_APP_CLASS class theApplicationClass    public:\
    linkedEntryPointClass setupListHead;\
    linkedEntryPointClass loopListHead;\
    linkedEntryPointClass systickListHead;\
    linkedEntryPointClass exceptionListHead;\
    MODCLASS_NAME(MODNAME) MODNAME##CompMod;\
    executionSystemClass* theExecutionSystemPtr;\
    theApplicationClass() :\
        MODNAME##CompMod(),\
        setupListHead(& MODNAME##CompMod, nullptr),\
        loopListHead(& MODNAME##CompMod, nullptr),\
        systickListHead(nullptr, nullptr),\
        exceptionListHead(&MODNAME##CompMod, nullptr)\
    {\
        theExecutionSystemPtr = &exeSystem;\
        theExecutionSystemPtr->LinkTheListsHead(\
            &setupListHead,\
            &loopListHead,\
            &systickListHead,\
            &exceptionListHead\
        );\
    }\
}
#define ccOS_APP_CLASS __ccOS_APP_CLASS

// ccACU_Class inherits from the SatComACS Compute Module
class ccACU_Class : public 
{

};


#endif // !__APP_SOLUTION__
