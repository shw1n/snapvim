// snapVim
// Give me vim bindings for window snap, plus Print Screen -> Media Play/Pause

// based on: https://www.unknowncheats.me/forum/c-and-c-/83707-setwindowshookex-example.html

#include <iostream>
#include <Windows.h>

HHOOK _hook;
KBDLLHOOKSTRUCT kbdStruct;

enum Snap { LEFT, RIGHT, UP, DOWN };

int snap(Snap snap);
int media_play_pause();

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {

	int retVal = 0;

	if (nCode >= 0) {
		if (wParam == WM_KEYDOWN)
		{
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			if (kbdStruct.vkCode == 0x48) {
				//'H'
				retVal = snap(LEFT);
			}
			else if (kbdStruct.vkCode == 0x4C) {
				// L
				retVal = snap(RIGHT);
			}
			else if (kbdStruct.vkCode == 0x4B) {
				// K
				retVal = snap(UP);
			}
			else if (kbdStruct.vkCode == 0x4A) {
				// J
				retVal = snap(DOWN);
			}
			else if (kbdStruct.vkCode == VK_SNAPSHOT) {
				// PrintScreen
				retVal = media_play_pause();
			}
		}
	}

	if (retVal == 1) {
		return 1;
	}

	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

int media_play_pause() {
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.wVk = VK_MEDIA_PLAY_PAUSE;
	ip.ki.dwFlags = 0;  // key press down
	SendInput(1, &ip, sizeof(INPUT));
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
	return 1;
}

int snap(Snap snap) {
	HWND hwnd;

	DWORD lwin_key = 0;
	DWORD rwin_key = 0;

	lwin_key = GetAsyncKeyState(VK_LWIN);
	rwin_key = GetAsyncKeyState(VK_RWIN);

	// Best part, shift doesn't need to be handled explicitly
	// It will just work w/ this translation (since you're pressing it anyway)
	if (lwin_key != 0 || rwin_key != 0) {
		
		// Get keypress struct ready
		INPUT ip;
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = 0;
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;
		ip.ki.dwFlags = 0;  // key press down

		if (snap == LEFT) {
			ip.ki.wVk = VK_LEFT;
			SendInput(1, &ip, sizeof(INPUT));
		}
		else if (snap == RIGHT) {
			ip.ki.wVk = VK_RIGHT;
			SendInput(1, &ip, sizeof(INPUT));
		}
		else if (snap == UP) {
			ip.ki.wVk = VK_UP;
			SendInput(1, &ip, sizeof(INPUT));
		}
		else if (snap == DOWN) {
			ip.ki.wVk = VK_DOWN;
			SendInput(1, &ip, sizeof(INPUT));
		}
		else {
			// Invalid state, should never get here
			return 0;
		}
		// Release key
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
		return 1;
	}

	return 0;
}


void SetHook() {
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0))) {
		MessageBox(NULL, L"(Failed to install shwin's hook!)", L"(ERROR)", MB_ICONERROR);
	}
}

void ReleaseHook() {
	UnhookWindowsHookEx(_hook);
}

int __stdcall WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       cmdShow)
{
	SetHook();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {

	}
}