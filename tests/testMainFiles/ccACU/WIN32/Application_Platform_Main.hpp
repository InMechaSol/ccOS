/** \file WIN32_Console.cpp
*   \brief WIN32 Console Implementation of ccNOos_Tests in c++

   Copyright 2021 InMechaSol, Inc

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

Notes:

*/

// This file should only ever be included by one file, PlatformApp_Serialization.cpp
// Therefore, it is not #ifdef guarded and will cause "multiple definition" errors if included more than once.
#include "PlatformApp_Serialization.hpp"


class ccACU_ApplicationClass   
{
public:


    // Linked Entry Points for execution of module entry points
    linkedEntryPointClass setupListHead; 
    linkedEntryPointClass loopListHead; 
    linkedEntryPointClass systickListHead; 
    linkedEntryPointClass exceptionListHead;

    SatComACS_class SatComACS_compMod;

    OSexecutionSystemClass* theExecutionSystemPtr;

    ccACU_ApplicationClass() :
    SatComACS_compMod(),
    setupListHead(&SatComACS_compMod, nullptr),
    loopListHead(&SatComACS_compMod, nullptr),
    systickListHead(nullptr, nullptr),
    exceptionListHead(&SatComACS_compMod, nullptr)
    {
        theExecutionSystemPtr = &exeSystem; 
        theExecutionSystemPtr->LinkTheListsHead(
                                                & setupListHead,
                                                & loopListHead,
                                                & systickListHead,
                                                & exceptionListHead
                                            );
    }

}
//ccOS_APP_CLASS




