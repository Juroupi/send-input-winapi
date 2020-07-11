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