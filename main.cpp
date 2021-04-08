/*

Copyright (c) 2021 Cohen ter Heide

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Cohen ter Heide
support@lumpology.com

*/

#include <iostream>
#include <string>
#include <cinttypes>
#include <windows.h>

bool keydown(int key) {
	return (GetAsyncKeyState(key) & 0x8000) != 0;
}


int main() {
	bool key_tap = false;
	wchar_t slot_digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	std::string clip_stored[10] = {"","","","","","","","","",""};
	while (true) {
		while (keydown(VK_LCONTROL) || keydown(VK_RCONTROL)) {
			for (uint8_t i = 0; i < 10; i++) {
				if (keydown('C') && keydown(slot_digits[i]) && key_tap==false) {
					std::cout << "combo: Ctrl + C + "<< std::to_string(i) << std::endl;
					Sleep(50);
					if (!OpenClipboard(NULL)) continue;
					clip_stored[i] = (char*)GetClipboardData(CF_TEXT);
					CloseClipboard();
					std::cout << clip_stored[i] << std::endl;
					key_tap = true;
					continue;
				} else if (keydown(slot_digits[i]) && key_tap==false) {
					std::cout << "combo: Ctrl + " << std::to_string(i) << std::endl;
					Sleep(50);
					size_t len = strlen(clip_stored[i].c_str()) + 1;
					HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
					memcpy(GlobalLock(hMem), clip_stored[i].c_str(), len);
					GlobalUnlock(hMem);
					OpenClipboard(NULL);
					EmptyClipboard();
					SetClipboardData(CF_TEXT, hMem);
					CloseClipboard();
					key_tap = true;
					continue;
				}
			}
			Sleep(50);
		}
		if (key_tap == true) key_tap = false;
	}
	return 0;
}