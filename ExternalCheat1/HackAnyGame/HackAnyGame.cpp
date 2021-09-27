
	#include "proc.h"

	int main()
	{
	// Initalize process variables, addresses and toggles.
	HANDLE hProcess = 0;

	uintptr_t moduleBase, localPlayerPtr, healthAddr;
	bool bHealth, bAmmo, bRecoil;

	// Set our buffer for the health & get process ID
	constexpr int newValue{ 1337 };
	DWORD processID = GetProcId(L"ac_client.exe");

	while (true)
	{
		if (processID)
		{
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, processID);
			moduleBase = GetModuleBaseAddress(processID, L"ac_client.exe");
			localPlayerPtr = moduleBase + 0x10f4f4;

			healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0xf8 });
			break;
		}
		else
		{
			// Keep trying to get the process ID
			std::cout << "Process not found, press enter to try again";
			getchar();
			processID = GetProcId(L"ac_client.exe");
		}
	}
	
	
	DWORD dwExit{ 0 };
	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
	{

	}
	