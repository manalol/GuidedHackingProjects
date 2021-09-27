#include "proc.h"


DWORD GetProcId(const wchar_t* processName)
{
	DWORD processID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 processEntry;
		processEntry.dwSize = sizeof(processEntry);

		if (Process32First(hSnap, &processEntry))
		{
			do
			{
				if (!_wcsicmp(processEntry.szExeFile, processName))
				{
					processID = processEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &processEntry));
		}
	}

	CloseHandle(hSnap);
	return processID;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* moduleName)
{
	uintptr_t moduleBaseAddress = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE || TH32CS_SNAPMODULE32, procId);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(moduleEntry);

		if (Module32First(hSnap, &moduleEntry))
		{
			do
			{
				if (!_wcsicmp(moduleEntry.szExePath, moduleName))
				{
					moduleBaseAddress = (uintptr_t) moduleEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &moduleEntry));
		}
	}
	CloseHandle(hSnap);
	return moduleBaseAddress; 
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	bool RPM_RESULT;

	for (int i = 0; i < offsets.size(); ++i)
	{
		RPM_RESULT = ReadProcessMemory(hProc, (BYTE*) addr, &addr, sizeof(uintptr_t), NULL); 

		if (RPM_RESULT)
		{
			std::cout << "Succesfully read pointer address, adding offset " << offsets[i] << "\n";
			addr += offsets[i];	
		}
		else
		{
			std::cerr << "Unable to read pointer address, function failed.\n";
			return NULL;
		}

	}

	return addr;	
}