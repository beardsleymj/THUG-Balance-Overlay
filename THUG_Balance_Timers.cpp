#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>


using namespace std;

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
uintptr_t GetManualTimerAddress(uintptr_t manual_timer_pointer, HANDLE hProcess);
uintptr_t GetGrindTimerAddress(uintptr_t THUG_Base_Address, HANDLE hProcess);

int main()
{
	HWND hWnd;
	HANDLE hProcess;
	DWORD pid;

	float manual_timer;
	float grind_timer;
	uintptr_t THUG_base_address;
	uintptr_t manual_timer_pointer;
	uintptr_t manual_timer_address;
	uintptr_t grind_timer_pointer;
	uintptr_t grind_timer_address;

	hWnd = FindWindowA(nullptr, ("THUG PRO"));
	if (!hWnd)
	{
		cout << "THUGPRO Not Detected." << endl;
		exit(-1);
	}

	GetWindowThreadProcessId(hWnd, &pid);

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (hProcess == NULL)
	{
		cout << "Cannot Open Process. " << endl;
		exit(-1);
	}

	THUG_base_address = GetModuleBaseAddress(pid, L"THUGPro.exe");
	manual_timer_address = GetManualTimerAddress(THUG_base_address, hProcess);      
	grind_timer_address = GetGrindTimerAddress(THUG_base_address, hProcess);

	
	while(true)
	{
		if (!ReadProcessMemory(hProcess, (void*)(manual_timer_address), (void*)&manual_timer, sizeof(manual_timer), NULL))
		{
			DWORD error = GetLastError();
			cout << "ERROR READING MANUAL TIMER" << error << endl;
		}

		if (!ReadProcessMemory(hProcess, (void*)(grind_timer_address), (void*)&grind_timer, sizeof(grind_timer), NULL))
		{
			DWORD error = GetLastError();
			cout << "ERROR READING GRIND TIMER" << error << endl;
		}


		Sleep(250);
		cout << "manual: " << manual_timer << endl;
		cout << "grind: " << grind_timer << endl;
	}

	CloseHandle(hProcess);
}

// From https://guidedhacking.com/threads/get-module-base-address-tutorial-dwgetmodulebaseaddress.5781/
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t GetManualTimerAddress(uintptr_t THUG_Base_Address, HANDLE hProcess)
{
	uintptr_t manual_timer_address = THUG_Base_Address + 0x002F0DC4;
	ReadProcessMemory(hProcess, (void*)(manual_timer_address), (void*)&manual_timer_address, sizeof(manual_timer_address), NULL);
	ReadProcessMemory(hProcess, (void*)(manual_timer_address + 0x10), (void*)&manual_timer_address, sizeof(manual_timer_address), NULL);
	ReadProcessMemory(hProcess, (void*)(manual_timer_address + 0x60), (void*)&manual_timer_address, sizeof(manual_timer_address), NULL);
	ReadProcessMemory(hProcess, (void*)(manual_timer_address + 0x70), (void*)&manual_timer_address, sizeof(manual_timer_address), NULL);
	manual_timer_address += 0xDC;
	return manual_timer_address;
}

uintptr_t GetGrindTimerAddress(uintptr_t THUG_Base_Address, HANDLE hProcess)
{
	uintptr_t grind_timer_address = THUG_Base_Address + 0x002F0DC8;
	ReadProcessMemory(hProcess, (void*)(grind_timer_address), (void*)&grind_timer_address, sizeof(grind_timer_address), NULL);
	ReadProcessMemory(hProcess, (void*)(grind_timer_address + 0x10), (void*)&grind_timer_address, sizeof(grind_timer_address), NULL);
	ReadProcessMemory(hProcess, (void*)(grind_timer_address + 0x10), (void*)&grind_timer_address, sizeof(grind_timer_address), NULL);
	ReadProcessMemory(hProcess, (void*)(grind_timer_address + 0x10), (void*)&grind_timer_address, sizeof(grind_timer_address), NULL);
	ReadProcessMemory(hProcess, (void*)(grind_timer_address + 0x10), (void*)&grind_timer_address, sizeof(grind_timer_address), NULL);
	ReadProcessMemory(hProcess, (void*)(grind_timer_address + 0x70), (void*)&grind_timer_address, sizeof(grind_timer_address), NULL);
	ReadProcessMemory(hProcess, (void*)(grind_timer_address + 0x1C4), (void*)&grind_timer_address, sizeof(grind_timer_address), NULL);
	grind_timer_address += 0xA4;

	return grind_timer_address;
}