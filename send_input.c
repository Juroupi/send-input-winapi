#include "send_input.h"

#include <windows.h>
#include <winuser.h>


void send_button_down(int button) {

	INPUT input = { 0 };

	input.type = INPUT_MOUSE;

	switch (button) {

		case MOUSE_BUTTON_LEFT:
			input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			break;

		case MOUSE_BUTTON_RIGHT:
			input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
			break;

		case MOUSE_BUTTON_MIDDLE:
			input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
			break;

		default:
			input.mi.dwFlags = MOUSEEVENTF_XDOWN;
			input.mi.mouseData = 1 << (button - MOUSE_BUTTON_X);
			break;
	}

	SendInput(1, &input, sizeof(INPUT));
}

void send_button_up(int button) {
	
	INPUT input = { 0 };

	input.type = INPUT_MOUSE;

	switch (button) {

		case MOUSE_BUTTON_LEFT:
			input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			break;

		case MOUSE_BUTTON_RIGHT:
			input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
			break;

		case MOUSE_BUTTON_MIDDLE:
			input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
			break;

		default:
			input.mi.dwFlags = MOUSEEVENTF_XUP;
			input.mi.mouseData = 1 << (button - MOUSE_BUTTON_X);
			break;
	}

	SendInput(1, &input, sizeof(INPUT));
}


void send_mouse_move(int dx, int dy) {

	INPUT input = { 0 };

	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.dx = dx;
	input.mi.dy = dy;

	SendInput(1, &input, sizeof(INPUT));
}


void send_mouse_wheel(int dx, int dy) {

	INPUT input = { 0 };

	input.type = INPUT_MOUSE;

	if (dy != 0) {

		input.mi.dwFlags = MOUSEEVENTF_WHEEL | MOUSEEVENTF_MOVE_NOCOALESCE;
		input.mi.mouseData = dy;

		SendInput(1, &input, sizeof(INPUT));
	}

	if (dx != 0) {

		input.mi.dwFlags = MOUSEEVENTF_HWHEEL | MOUSEEVENTF_MOVE_NOCOALESCE;
		input.mi.mouseData = dx;

		SendInput(1, &input, sizeof(INPUT));
	}
}


void send_key_down(int keycode, int extended) {

	INPUT input = { 0 };

	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE | (extended ? KEYEVENTF_EXTENDEDKEY : 0);
	input.ki.wScan = MapVirtualKey(keycode, MAPVK_VK_TO_VSC);

	SendInput(1, &input, sizeof(INPUT));
}

void send_key_up(int keycode, int extended) {

	INPUT input = { 0 };

	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP | (extended ? KEYEVENTF_EXTENDEDKEY : 0);
	input.ki.wScan = MapVirtualKey(keycode, MAPVK_VK_TO_VSC);

	SendInput(1, &input, sizeof(INPUT));
}


int char_to_keycode(int c, int* shift, int* ctrl, int* alt) {
	
	SHORT ret = VkKeyScanA(c);

	if (ret < 0) {
		return -1;
	}

	int keycode = ret & 0xFF;

	ret >>= 8;

	*shift = ret & 1;
	*ctrl  = ret & 2;
	*alt   = ret & 4;

	return keycode;
}

int wchar_to_keycode(int wc, int* shift, int* ctrl, int* alt) {

	SHORT ret = VkKeyScanW(wc);

	if (ret < 0) {
		return -1;
	}

	int keycode = ret & 0xFF;

	ret >>= 8;

	*shift = ret & 1;
	*ctrl  = ret & 2;
	*alt   = ret & 4;

	return keycode;
}


void send_char(char c) {

	int keycode, shift, ctrl, alt;

	keycode = char_to_keycode(c, &shift, &ctrl, &alt);

	if (keycode > 0) {

		if (ctrl)  send_key_down(VK_LCONTROL, 1);
		if (alt)   send_key_down(VK_RMENU, 1);
		if (shift) send_key_down(VK_LSHIFT, 1);

		send_key_down(keycode, 0);

		if (shift) send_key_up(VK_LSHIFT, 1);
		if (alt)   send_key_up(VK_RMENU, 1);
		if (ctrl)  send_key_up(VK_LCONTROL, 1);
	}
}


void send_string(char* str) {

	char c;

	int keycode;
	
	int shift, shift2 = 0;
	int ctrl, ctrl2 = 0;
	int alt, alt2 = 0;

	while ((c = *str++) != '\0') {

		keycode = char_to_keycode(c, &shift, &ctrl, &alt);

		if (keycode > 0) {

			if (ctrl) {
				send_key_down(keycode, 0);
			}
		}
	}
}