#include "UIServerClass.hpp"
#include "osApp_Solution.hpp"


char* UI_ServerClass::IDString()
{
	return InstanceIDstring;
}
int UI_ServerClass::InstanceCount = 0;
UI_ServerClass::UI_ServerClass(struct UIServerStruct* dataIn, ccACU_Class* ccACUPtrIn) : exe_thread_class(&dataIn->compModData),
LCDKeyPad_NBSerial(&LCDKeyPadPortParams),
ConsoleMenu_tcpServer1(&ConsoleMenuTCPServerStruct1),
ConsoleMenu_tcpServer2(&ConsoleMenuTCPServerStruct2),
theRootMenuNode(ccACUPtrIn),
theMainMenuNode(ccACUPtrIn),
thesatcomacsNode(ccACUPtrIn)
{
	InstanceCount++;
	InstanceIDstring = (char*)"UIServer_00";
	ccACUPtr = ccACUPtrIn;
	data = dataIn;


    ConsoleMenuTCPServerStruct1.listenPort = 55500;
    ConsoleMenuTCPServerStruct2.listenPort = 55600;

	// This is an ideal spot to link IO devices with api modules
	((SatComACSStruct*)ccACUPtrIn->getModuleDataPtr())->LCDKeyPad.devptr = LCDKeyPad_NBSerial.GetDevPtr();
	((SatComACSStruct*)ccACUPtrIn->getModuleDataPtr())->ConsoleMenu.devptr = &ConsoleMenuTCPServerStruct1.tcpData.devdata;
	ccACUPtrIn->data->ConsoleMenu2.devptr = &ConsoleMenuTCPServerStruct2.tcpData.devdata;

}
int UI_ServerClass::mod_setup() 
{ 
    ConsoleMenu_tcpServer1.OpenDev();
    ConsoleMenu_tcpServer2.OpenDev();
    ccACUPtr->SatComACSData.ConsoleMenu.viewFormatIndex = cV_Console;
    ccACUPtr->data->ConsoleMenu2.viewFormatIndex = cV_Console;
	return RETURN_ERROR; 
}

void writeHelpString(struct uiStruct* uiStructPtrIn)
{
    if (uiStructPtrIn == nullptr)
    {
        uiStructPtrIn->lines2print = 0;
        return;
    }
    if (uiStructPtrIn->showHelp == ui8TRUE)
    {
        OPENSWITCH(uiStructPtrIn)
        case 0:
            PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), "ccACU", "Help", terminalSlashes()   END_MENU_LN;
        case 1:
            PRINT_MENU_LN  "\n%sACU; \t\tto display Main Menu", cursorString(0)   END_MENU_LN;        
        default:        
        CLOSESWITCH(uiStructPtrIn)

    }
    else
        uiStructPtrIn->lines2print = 0;
}

#define MENU ccACUPtr->SatComACSData.ConsoleMenu
#define TEMPIODEV ConsoleMenuTCPStructTemp1.devdata
#define TCPSERVER ConsoleMenu_tcpServer1
#define TCPDATA ConsoleMenuTCPServerStruct1.tcpData
#define IODEVPTR TCPSERVER.GetDevPtr() 
#define TCPSTATE lastTCPConsoleState1
void UI_ServerClass::readTCP1()
{
    // non-blocking server mainatainance
    TCPSERVER.ShutdownRestartServerSocket();

    // non-blocking check for client connection
    TCPSERVER.ServerListenAccept();

    // on each client connection, trigger menu write and show help
    if (TCPSTATE != TCPSERVER.getTCPStatus())
    {
        TCPSTATE = TCPSERVER.getTCPStatus();
        if (TCPSTATE == tcpstat_connected)
        {
            MENU.showHelp = ui8TRUE;
            IODEVPTR->triggerWriteOperation = ui8TRUE;
        }
    }

    // non-blocking check for client packet
    if (TCPSERVER.ReadDev() > 0)
    {
        IODEVPTR->newDataReadIn = ui8TRUE;
        MENU.parseIndex = 0;
    }
}
void UI_ServerClass::writeTCP1()
{
    if (MENU.clearScreen) {
        TEMPIODEV.numbytes2Write = IODEVPTR->numbytes2Write;
        stringInit(TEMPIODEV.outbuff.charbuff, IODEVPTR->outbuff.charbuff);
        stringInit(IODEVPTR->outbuff.charbuff, terminalClearString());
        IODEVPTR->numbytes2Write = stringLength(terminalClearString());
        TCPWrite(&TCPDATA);
        stringInit(IODEVPTR->outbuff.charbuff, TEMPIODEV.outbuff.charbuff);
        IODEVPTR->numbytes2Write = TEMPIODEV.numbytes2Write;
        MENU.clearScreen = ui8FALSE;
    }
    if (TCPSERVER.WriteDev() > 0);
    if (MENU.showPrompt) {
        TEMPIODEV.numbytes2Write = IODEVPTR->numbytes2Write;
        stringInit(TEMPIODEV.outbuff.charbuff, IODEVPTR->outbuff.charbuff);
        stringInit(IODEVPTR->outbuff.charbuff, terminalPromptString(MENU.currentUserLevel));
        IODEVPTR->numbytes2Write = stringLength(terminalPromptString(MENU.currentUserLevel));
        TCPWrite(&TCPDATA);
        stringInit(IODEVPTR->outbuff.charbuff, TEMPIODEV.outbuff.charbuff);
        IODEVPTR->numbytes2Write = TEMPIODEV.numbytes2Write;
        MENU.showPrompt = ui8FALSE;
    }
}
#undef MENU
#undef TEMPIODEV
#undef TCPSERVER
#undef IODEVPTR
#undef TCPSTATE
#undef TCPDATA

#define MENU ccACUPtr->data->ConsoleMenu2
#define TEMPIODEV ConsoleMenuTCPStructTemp2.devdata
#define TCPSERVER ConsoleMenu_tcpServer2
#define TCPDATA ConsoleMenuTCPServerStruct2.tcpData
#define IODEVPTR TCPSERVER.GetDevPtr() 
#define TCPSTATE lastTCPConsoleState2
void UI_ServerClass::readTCP2()
{
    // non-blocking server mainatainance
    TCPSERVER.ShutdownRestartServerSocket();

    // non-blocking check for client connection
    TCPSERVER.ServerListenAccept();

    // on each client connection, trigger menu write and show help
    if (TCPSTATE != TCPSERVER.getTCPStatus())
    {
        TCPSTATE = TCPSERVER.getTCPStatus();
        if (TCPSTATE == tcpstat_connected)
        {
            MENU.showHelp = ui8TRUE;
            IODEVPTR->triggerWriteOperation = ui8TRUE;
        }
    }

    // non-blocking check for client packet
    if (TCPSERVER.ReadDev() > 0)
    {
        IODEVPTR->newDataReadIn = ui8TRUE;
        MENU.parseIndex = 0;
    }
}
void UI_ServerClass::writeTCP2()
{
    if (MENU.clearScreen) {
        TEMPIODEV.numbytes2Write = IODEVPTR->numbytes2Write;
        stringInit(TEMPIODEV.outbuff.charbuff, IODEVPTR->outbuff.charbuff);
        stringInit(IODEVPTR->outbuff.charbuff, terminalClearString());
        IODEVPTR->numbytes2Write = stringLength(terminalClearString());
        TCPWrite(&TCPDATA);
        stringInit(IODEVPTR->outbuff.charbuff, TEMPIODEV.outbuff.charbuff);
        IODEVPTR->numbytes2Write = TEMPIODEV.numbytes2Write;
        MENU.clearScreen = ui8FALSE;
    }
    if (TCPSERVER.WriteDev() > 0);
    if (MENU.showPrompt) {
        TEMPIODEV.numbytes2Write = IODEVPTR->numbytes2Write;
        stringInit(TEMPIODEV.outbuff.charbuff, IODEVPTR->outbuff.charbuff);
        stringInit(IODEVPTR->outbuff.charbuff, terminalPromptString(MENU.currentUserLevel));
        IODEVPTR->numbytes2Write = stringLength(terminalPromptString(MENU.currentUserLevel));
        TCPWrite(&TCPDATA);
        stringInit(IODEVPTR->outbuff.charbuff, TEMPIODEV.outbuff.charbuff);
        IODEVPTR->numbytes2Write = TEMPIODEV.numbytes2Write;
        MENU.showPrompt = ui8FALSE;
    }
}
#undef MENU
#undef TEMPIODEV
#undef TCPSERVER
#undef IODEVPTR
#undef TCPSTATE
#undef TCPDATA

int UI_ServerClass::mod_loop() 
{ 
#define MENU ccACUPtr->SatComACSData.ConsoleMenu
#define IODEVPTR MENU.devptr

    readTCP1();
    if (MENU.devptr->newDataReadIn)
    {
        data->uiPtrArray[1]->getActiveMenuPtr()->parseInput(data->uiPtrArray[1]);
        IODEVPTR->triggerWriteOperation = ui8TRUE;
        IODEVPTR->newDataReadIn = ui8FALSE;
    }
    if (MENU.devptr->triggerWriteOperation)
    {
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        MENU.clearScreen = ui8TRUE;
        OPENDOWHILE((&MENU))
            data->uiPtrArray[1]->getActiveMenuPtr()->printMenu(data->uiPtrArray[1]);
        CLOSEDOWHILE2((&MENU), writeTCP1)        
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        OPENDOWHILE(&MENU)
            writeHelpString(&MENU);
        CLOSEDOWHILE2((&MENU), writeTCP1)
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        OPENDOWHILE(&MENU)
            writeUIHelpString(&MENU);
        CLOSEDOWHILE2((&MENU), writeTCP1)
        MENU.showPrompt = ui8TRUE;
        IODEVPTR->outbuff.charbuff[0] = 0x00;
        IODEVPTR->triggerWriteOperation = ui8FALSE;
        writeTCP1();        
    }

#undef MENU
#undef IODEVPTR
#define MENU ccACUPtr->data->ConsoleMenu2
#define IODEVPTR MENU.devptr

    readTCP2();    
    if (MENU.devptr->newDataReadIn)
    {
        data->uiPtrArray[2]->getActiveMenuPtr()->parseInput(data->uiPtrArray[2]);
        IODEVPTR->triggerWriteOperation = ui8TRUE;
        IODEVPTR->newDataReadIn = ui8FALSE;
    }
    if (MENU.devptr->triggerWriteOperation)
    {
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        MENU.clearScreen = ui8TRUE;
        OPENDOWHILE((&MENU))
            data->uiPtrArray[2]->getActiveMenuPtr()->printMenu(data->uiPtrArray[2]);
        CLOSEDOWHILE2((&MENU), writeTCP2)
            MENU.lines2print = 1;
        MENU.linesprinted = 0;
        OPENDOWHILE(&MENU)
            writeHelpString(&MENU);
        CLOSEDOWHILE2((&MENU), writeTCP2)
        MENU.lines2print = 1;
        MENU.linesprinted = 0;
        OPENDOWHILE(&MENU)
            writeUIHelpString(&MENU);
        CLOSEDOWHILE2((&MENU), writeTCP2)
            MENU.showPrompt = ui8TRUE;
        IODEVPTR->outbuff.charbuff[0] = 0x00;
        IODEVPTR->triggerWriteOperation = ui8FALSE;
        writeTCP2();
    }

#undef MENU

	return RETURN_ERROR; 
}
void UI_ServerClass::mod_systick() 
{

}
int UI_ServerClass::mod_excphandler()
{ 
	return RETURN_ERROR; 
}


menuNode* UI_ServerClass::fromMenuIndex(int menuIndexIn)
{
    return &ccACUPtr->getUIServerPtr()->thesatcomacsNode;
}

void parseGroupccACU(ccACU_Class* ccACUPtrIn, consoleMenuClass* uiMenuPtrIn)
{
    struct uiStruct* uiStructPtrIn = uiMenuPtrIn->getUIdataPtr();
    int curMenIndex = uiStructPtrIn->currentMenuIndex;
    parseSatComACSMenuAPI(((SatComACSStruct*)ccACUPtrIn->getModuleDataPtr()), uiStructPtrIn);

    // if satcomacs current menu index has changed, set active menu node ptr accordingly
    if(curMenIndex!= uiStructPtrIn->currentMenuIndex)
        uiMenuPtrIn->setActiveMenuPtr(ccACUPtrIn->getUIServerPtr()->fromMenuIndex(uiStructPtrIn->currentMenuIndex));
}
void parseccACUMenuAPI(ccACU_Class* ccACUPtrIn, consoleMenuClass* uiMenuPtrIn)
{
    struct uiStruct* uiStructPtrIn = uiMenuPtrIn->getUIdataPtr();
    
    OPENIF2("ACU", ccACUPtrIn->getUIServerPtr()->theMainMenuNode)
        parseGroupccACU(ccACUPtrIn, uiMenuPtrIn);
    CLOSEIF("ACU", ccACUPtrIn->getUIServerPtr()->theMainMenuNode)
        parseGroupccACU(ccACUPtrIn, uiMenuPtrIn);
}
void writeccACUMenuScreenConsole(ccACU_Class* ccACUPtrIn, consoleMenuClass* uiMenuPtrIn)
{
    struct uiStruct* uiStructPtrIn = uiMenuPtrIn->getUIdataPtr();

    OPENSWITCH(uiStructPtrIn)
	case 0:
        PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), "ccACU", "Main", terminalSlashes()	END_MENU_LN;
    case 1:
        PRINT_MENU_LN  "\n%sModem", cursorString(uiStructPtrIn->cursorIndex == 0)						END_MENU_LN;
    case 2:
        PRINT_MENU_LN  "\n%sManaged Switch", cursorString(uiStructPtrIn->cursorIndex == 1)				END_MENU_LN;
    case 3:
        PRINT_MENU_LN  "\n%sOS Execution System", cursorString(uiStructPtrIn->cursorIndex == 2)			END_MENU_LN;
    case 4:
        PRINT_MENU_LN  "\n%sSatComACS", cursorString(uiStructPtrIn->cursorIndex == 3)						END_MENU_LN;
    default:
    CLOSESWITCH(uiStructPtrIn)
}
void writeccACUMenuScreenLCDBig(ccACU_Class* ccACUPtrIn, consoleMenuClass* uiMenuPtrIn)
{
    struct uiStruct* uiStructPtrIn = uiMenuPtrIn->getUIdataPtr();

    OPENSWITCH(uiStructPtrIn)
    case 0:
        PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), "ccACU", "Main", terminalSlashes()	END_MENU_LN;
    case 1:
        PRINT_MENU_LN  "\n%s", cursorString(uiStructPtrIn->cursorIndex == 0)						END_MENU_LN;
    default:
    CLOSESWITCH(uiStructPtrIn)
}
void writeccACUMenuScreenLCDSmall(ccACU_Class* ccACUPtrIn, consoleMenuClass* uiMenuPtrIn)
{
    struct uiStruct* uiStructPtrIn = uiMenuPtrIn->getUIdataPtr();

    OPENSWITCH(uiStructPtrIn)
    case 0:
        PRINT_MENU_LN  "\n%s %s - %s Menu %s", terminalSlashes(), "ccACU", "Main", terminalSlashes()	END_MENU_LN;
    case 1:
        PRINT_MENU_LN  "\n%s", cursorString(uiStructPtrIn->cursorIndex == 0)						END_MENU_LN;
    default:
    CLOSESWITCH(uiStructPtrIn)
}




UI_ServerClass::ccacu_menuNode::ccacu_menuNode(ccACU_Class* acuptrIn):satcomacs_menuNode(acuptrIn)
{
    acuptr = acuptrIn;
}
void UI_ServerClass::ccacu_menuNode::parseInput(consoleMenuClass* uiMenuPtrIn)
{
    parseccACUMenuAPI( acuptr, uiMenuPtrIn );
}

UI_ServerClass::satcomacsNode::satcomacsNode(ccACU_Class* acuptrIn):UI_ServerClass::ccacu_menuNode(acuptrIn)
{

}
void UI_ServerClass::satcomacsNode::printMenu(consoleMenuClass* uiMenuPtrIn)
{
    printM_SatComACS(acuptr->getModuleDataPtr(), uiMenuPtrIn->getUIdataPtr());
}

UI_ServerClass::mainMenuNode::mainMenuNode(ccACU_Class* acuptrIn) :UI_ServerClass::ccacu_menuNode(acuptrIn)
{

}
void UI_ServerClass::mainMenuNode::printMenu(consoleMenuClass* uiMenuPtrIn)
{
    struct uiStruct* uiDataPtrIn = uiMenuPtrIn->getUIdataPtr();

    switch (uiDataPtrIn->viewFormatIndex)
    {
    case cV_Console: writeccACUMenuScreenConsole(acuptr, uiMenuPtrIn); break;
    case cv_LCD4_40: writeccACUMenuScreenLCDBig(acuptr, uiMenuPtrIn); break;
    case cv_LCD2_40: writeccACUMenuScreenLCDSmall(acuptr, uiMenuPtrIn); break;
    }
}

UI_ServerClass::rootMenuNode::rootMenuNode(ccACU_Class* acuptrIn) :UI_ServerClass::ccacu_menuNode(acuptrIn)
{

}
void UI_ServerClass::rootMenuNode::printMenu(consoleMenuClass* uiMenuPtrIn)
{
    
}

