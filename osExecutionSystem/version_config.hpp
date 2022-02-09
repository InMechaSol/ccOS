#ifndef __ccOSVerConfig__
#define __ccOSVerConfig__

////////////////////////////////
// Bring In ccNOos version config header
#include "console_menu.h"
#if PLATFORM_NAME==ccOS
#else
#error "ccOS Platform NOT Selected for Compile!"
#endif

#define ccOS_MajorVer 0
#define ccOS_MinorVer 0
#define ccOS_BuildNumber 0

#define ccOS_DevString dev
#define ccOS_VerDate 08FEB2021

#ifdef ccOS_ReleaseBuild
#define ccOS_VersionNumber ccOS_MajorVer.ccOS_MinorVer.ccOS_BuildNumber
#endif
#ifndef ccOS_ReleaseBuild
#define ccOS_VersionNumber ccOS_MajorVer.ccOS_MinorVer.ccOS_BuildNumber-ccOS_DevString
#endif

const char* ccOSccOS_VerString();
const char* ccOSccOS_VerDateString();

#define ccOSVersionsTemplate \
    const char* ccOSccOS_VerString()\
    {\
        return xstr(ccOS_VersionNumber);\
    }\
    const char* ccOSccOS_VerDateString()\
    {\
        return xstr(ccOS_VerDate);\
    }\
    ccNOosVersionsTemplate



#endif //!__ccOSVerConfig__