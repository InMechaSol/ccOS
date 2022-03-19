#ifndef __UISERVER__
#define __UISERVER__

// include the SatComACS Application and Serialization from ccNOos Layer
#include "PlatformApp_Serialization.h"
// include ccOS execution system and exe thread module
//#include "os_execution_system.hpp"
#include "exe_thread.hpp"
// include ccOS comm modules and libraries
#include "tcp_comms.h"
#include "serial_comms.h"

class ccACU_Class;

#define MAXUIs (3u)

// Data Structure - UI API Module
struct UIServerStruct
{
	struct computeModuleStruct compModData = CreateComputeModuleStruct();
	consoleMenuClass* uiPtrArray[MAXUIs] = {nullptr, nullptr, nullptr};// array of to be linked in constructor
};



// Exe Thread - UI API Module
class UI_ServerClass : public exe_thread_class
{
protected:
	char* InstanceIDstring;
	struct UIServerStruct* data;
	ccACU_Class* ccACUPtr;

	// LCD Keypad Device
	struct portParametersStruct LCDKeyPadPortParams;
	nbserial_class LCDKeyPad_NBSerial;

	// Console Menu 1 Device
	struct tcpServerStruct ConsoleMenuTCPServerStruct1 = createTCPServerStruct();
	struct tcpStruct ConsoleMenuTCPStructTemp1 = createTCPStruct();
	tcp_server_class ConsoleMenu_tcpServer1;
	enum tcpServerClientStatus lastTCPConsoleState1 = tcpstat_uninitialized;

	// Console Menu 2 Device
	struct tcpServerStruct ConsoleMenuTCPServerStruct2 = createTCPServerStruct();;
	struct tcpStruct ConsoleMenuTCPStructTemp2 = createTCPStruct();;
	tcp_server_class ConsoleMenu_tcpServer2;
	enum tcpServerClientStatus lastTCPConsoleState2 = tcpstat_uninitialized;

	void readTCP1();
	void writeTCP1();
	void readTCP2();
	void writeTCP2();

	

public:
	menuNode* fromMenuIndex(int menuIndexIn);
	static int InstanceCount;
	char* IDString();
	int mod_setup();
	int mod_loop();
	void mod_systick();
	int mod_excphandler();
	UI_ServerClass(struct UIServerStruct* dataIn, ccACU_Class* ccACUPtrIn);

	class ccacu_menuNode :public satcomacs_menuNode
	{
	protected:
		ccACU_Class* acuptr = nullptr;
	public:
		ccacu_menuNode(ccACU_Class* acuptrIn);
		void parseInput(consoleMenuClass* uiMenuPtrIn);
	};
	class rootMenuNode : public ccacu_menuNode
	{
	public: 
		rootMenuNode(ccACU_Class* acuptrIn);
		void printMenu(consoleMenuClass* uiMenuPtrIn);
	};
	rootMenuNode theRootMenuNode;
	class mainMenuNode : public ccacu_menuNode
	{
	public:
		mainMenuNode(ccACU_Class* acuptrIn);
		void printMenu(consoleMenuClass* uiMenuPtrIn);
	};
	mainMenuNode theMainMenuNode;
	class satcomacsNode : public ccacu_menuNode
	{
	public:
		satcomacsNode(ccACU_Class* acuptrIn);
		void printMenu(consoleMenuClass* uiMenuPtrIn);
	};
	satcomacsNode thesatcomacsNode;
};

#endif // !__UISERVER__