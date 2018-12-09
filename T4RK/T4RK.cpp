#include "pch.h"
#include <iostream>
#include <thread>
#include "Memory.h"
#include "Offsets.hpp"

#define VERSION 0.1;

DWORD CLIENT_BASE = 0;

float R = 255;
float G = 255;
float B = 255;
float A = 255;

bool GLOW = false;

void GlowESP() {
	std::cout << "ESP Thread is ready!" << std::endl;
	while (true) {
		if (GLOW == true) {
			for (int i = 0; i < 32; i++) {
				DWORD entitybase = ProcessMemory::RPM<DWORD>(CLIENT_BASE + signatures::dwEntityList + (i * 0x10));
				DWORD glowIndex = ProcessMemory::RPM<DWORD>(entitybase + netvars::m_iGlowIndex);
				DWORD glowObject = ProcessMemory::RPM<DWORD>(CLIENT_BASE + signatures::dwGlowObjectManager);
				int Team = ProcessMemory::RPM<DWORD>(entitybase + netvars::m_iTeamNum);

				if (Team == 3) {
					R = 0;
					G = 0;
					B = 255;
				}

				if (Team == 2) {
					R = 255;
					G = 0;
					B = 0;
				}

				if (ProcessMemory::WPM<float>(R / 255, (glowObject + ((glowIndex * 0x38) + 0x4))) != 0) {
					ProcessMemory::WPM<float>(G / 255, (glowObject + ((glowIndex * 0x38) + 0x8)));
					ProcessMemory::WPM<float>(B / 255, (glowObject + ((glowIndex * 0x38) + 0xC)));
					ProcessMemory::WPM<float>(A / 255, (glowObject + ((glowIndex * 0x38) + 0x10)));
					ProcessMemory::WPM<bool>(true, (glowObject + ((glowIndex * 0x38) + 0x24)));
					ProcessMemory::WPM<bool>(false, (glowObject + ((glowIndex * 0x38) + 0x25)));
				}
			}
		}
	}
}

int main()
{
	std::cout << "[ T4RK External CSGO Cheat Version 0.1 by Cool Guy]" << std::endl << std::endl;
	std::cout << "Attaching to csgo!\n" << std::endl;
	ProcessMemory::Attach(L"csgo.exe");
	std::cout << "Getting client_panorama.dll address!" << std::endl << std::endl;
	CLIENT_BASE = ProcessMemory::FindModule(L"client_panorama.dll");
	std::cout << std::endl << "F1 | Open/Close ESP\nF2 | Close Program" << std::endl;

	std::thread espthread(GlowESP);

	while (true) {
		if (GetAsyncKeyState(VK_F1)) {
			GLOW = !GLOW;
			if (GLOW == true) {
				std::cout << "ESP is open!" << std::endl;
			}
			else {
				std::cout << "ESP is closed!" << std::endl;
			}
		}
		else if (GetAsyncKeyState(VK_F2)) {
			std::cout << std::endl << "Closing in 5 seconds!" << std::endl;
			ProcessMemory::Deattach();
			Sleep(5000);
			exit(0);
		}
		Sleep(200);
	}

	ProcessMemory::Deattach();

}