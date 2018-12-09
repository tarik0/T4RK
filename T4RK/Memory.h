#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

#ifndef _MEMORY

namespace ProcessMemory {
	DWORD pID;
	HANDLE hProc;

	bool Attach(const wchar_t* processName) {
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (snap == INVALID_HANDLE_VALUE) {
			std::cout << "Invalid Snap Value!" << std::endl;
			return false;
		}

		if (Process32First(snap, &entry)) {
			if (wcscmp(entry.szExeFile, processName) == 0) {
				std::cout << "Process found!" << std::endl;
				pID = entry.th32ProcessID;
				CloseHandle(snap);
				hProc = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pID);
				return true;
			}
			while (Process32Next(snap, &entry)) {
				if (wcscmp(entry.szExeFile, processName) == 0) {
					std::cout << "Process found!" << std::endl;
					pID = entry.th32ProcessID;
					CloseHandle(snap);
					hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
					return true;
				}
			}
		}
		std::cout << "Process not found!" << std::endl;
		return false;
	};

	void Deattach() {
		CloseHandle(hProc);
	};

	DWORD FindModule(const wchar_t* moduleName) {
		MODULEENTRY32 entry;
		entry.dwSize = sizeof(MODULEENTRY32);

		HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);

		if (Module32First(snap, &entry)) {
			if (wcscmp(entry.szModule, moduleName) == 0) {
				std::cout << "Module found!" << std::endl;
				CloseHandle(snap);
				return (DWORD)entry.modBaseAddr;
			}
			while (Module32Next(snap, &entry)) {
				if (wcscmp(entry.szModule, moduleName) == 0) {
					std::cout << "Module found!" << std::endl;
					CloseHandle(snap);
					return (DWORD)entry.modBaseAddr;
				}
			}
		}
		std::cout << "Module not found!" << std::endl;
		return 0;
	};

	template<class dType>
	dType RPM(DWORD address)
	{
		dType buffer;
		ReadProcessMemory(hProc,(LPVOID)address,&buffer,sizeof(buffer),NULL);
		return buffer;
	}

	template<class dType>
	bool WPM(dType value, DWORD address)
	{
		return WriteProcessMemory(hProc, (LPVOID)address, &value, sizeof(value), 0);
	}

};



#endif // !_MEMORY
