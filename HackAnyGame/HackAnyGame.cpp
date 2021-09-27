
#include "proc.h"

int main()
{
	
	// Get process ID
	DWORD processID = GetProcId(L"ac_client.exe");
	std::cout << "Process ID: " << processID << std::endl;

	// Get module base address
	uintptr_t modBaseAddress = GetModuleBaseAddress(processID, L"ac_client.exe");
	std::cout << "Module base address: 0x" << std::hex << modBaseAddress << std::endl;

	// Get a handle to the process
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);

	// Resolve (find) base address of pointer chain
	uintptr_t dynamicBasePointerAddress = modBaseAddress + 0x10F4F4;

	std::cout << "Base pointer address: 0x" << dynamicBasePointerAddress << std::endl;
	// Resolve base address of ammo pointer chain
	uintptr_t ammoAddress = FindDMAAddy(hProcess, dynamicBasePointerAddress, std::vector<unsigned int>{0x374, 0x14, 0x0});
	std::cout << "Ammo address: 0x" << ammoAddress << std::endl;

	// Read ammo, edit it then read it again
	int currAmmo;
	int newAmmo{ 9999 };

	ReadProcessMemory(hProcess, (BYTE*)ammoAddress, &currAmmo, sizeof(currAmmo), 0);
	std::cout << "Current ammo amount: " << std::dec << currAmmo << std::endl;
	WriteProcessMemory(hProcess, (BYTE*)ammoAddress, &newAmmo, sizeof(newAmmo), 0);
	std::cout << "Changing ammo..." << std::endl;
	ReadProcessMemory(hProcess, (BYTE*)ammoAddress, &currAmmo, sizeof(currAmmo), 0);
	std::cout << "Current ammo amount: " << currAmmo << std::endl;

}