#include "send_input.h"

#include <stdlib.h>
#include <math.h>
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


int char_to_keycode(char c, int* shift, int* ctrl, int* alt) {
	
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

int wchar_to_keycode(wchar_t wc, int* shift, int* ctrl, int* alt) {

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

		if (ctrl)  send_key_down(VK_LCONTROL, 0);
		if (alt)   send_key_down(VK_RMENU, 0);
		if (shift) send_key_down(VK_LSHIFT, 0);

		send_key_down(keycode, 0);

		if (shift) send_key_up(VK_LSHIFT, 0);
		if (alt)   send_key_up(VK_RMENU, 0);
		if (ctrl)  send_key_up(VK_LCONTROL, 0);
	}
}

void send_wchar(wchar_t wc) {

	int keycode, shift, ctrl, alt;

	keycode = wchar_to_keycode(wc, &shift, &ctrl, &alt);

	if (keycode > 0) {

		if (ctrl)  send_key_down(VK_LCONTROL, 0);
		if (alt)   send_key_down(VK_RMENU, 0);
		if (shift) send_key_down(VK_LSHIFT, 0);

		send_key_down(keycode, 0);

		if (shift) send_key_up(VK_LSHIFT, 0);
		if (alt)   send_key_up(VK_RMENU, 0);
		if (ctrl)  send_key_up(VK_LCONTROL, 0);
	}
}


int wpm_to_mspc(float words_per_minute) {
	return (int)((words_per_minute > 0) ? (60000.0f / (words_per_minute * 5.0f)) : 0);
}


static int random_ms(int ms, int dms) {
	if (dms == 0) return ms;
	float x = rand() / (float)(RAND_MAX + 1);
	float dt = logf(x / (1.0f - x)) * 0.1f + 0.5f;
	if (dt > 1.0f || dt < 0.0f) dt = rand() / (float)RAND_MAX;
	return (int)(2.0f * dms * dt) + ms - dms;
}


void sleep_ms(int ms, int dms) {
	int t = random_ms(ms, dms);
	if (t > 0) Sleep(t);
}


void send_string(const char* str, int ms_per_char, int dms) {

	char c;

	int keycode;
	
	int shift, shift_old = 0;
	int ctrl, ctrl_old = 0;
	int alt, alt_old = 0;

	if (dms > ms_per_char) {
		dms = ms_per_char;
	}

	int mspc4 = ms_per_char / 4;
	int dms4 = dms / 4;

	int mspc_released = mspc4;
	int dms_released = dms4;

	int mspc_pressed = ms_per_char  - mspc4;
	int dms_pressed = dms - dms4;

	if (mspc_pressed + dms_pressed > 600) {

		if (dms_pressed > 600) {
			dms_pressed = 300;
		}

		mspc_pressed = 600 - dms_pressed;

		mspc_released = ms_per_char - mspc_pressed;
		dms_released = dms - dms_pressed;
	}

	while ((c = *str++) != '\0') {

		keycode = char_to_keycode(c, &shift, &ctrl, &alt);
		
		if (keycode > 0) {

			if (ctrl) {
				if (!ctrl_old) {
					sleep_ms(mspc4, dms4);
					send_key_down(VK_LCONTROL, 0);
				}
			}
			else if (ctrl_old) {
				send_key_up(VK_LCONTROL, 0);
			}

			if (alt) {
				if (!alt_old) {
					sleep_ms(mspc4, dms4);
					send_key_down(VK_RMENU, 0);
				}
			}
			else if (alt_old) {
				send_key_up(VK_RMENU, 0);
			}

			if (shift) {
				if (!shift_old) {
					sleep_ms(mspc4, dms4);
					send_key_down(VK_LSHIFT, 0);
				}
			}
			else if (shift_old) {
				send_key_up(VK_LSHIFT, 0);
			}
			
			sleep_ms(mspc_released, dms_released);

			send_key_down(keycode, 0);

			sleep_ms(mspc_pressed, dms_pressed);

			send_key_up(keycode, 0);

			ctrl_old = ctrl;
			alt_old = alt;
			shift_old = shift;
		}
	}
}


void send_wstring(const wchar_t* wstr, int ms_per_char, int dms) {

	wchar_t wc;

	int keycode;

	int shift, shift_old = 0;
	int ctrl, ctrl_old = 0;
	int alt, alt_old = 0;

	if (dms > ms_per_char) {
		dms = ms_per_char;
	}

	int mspc4 = ms_per_char / 4;
	int dms4 = dms / 4;

	int mspc_released = mspc4;
	int dms_released = dms4;

	int mspc_pressed = ms_per_char - mspc4;
	int dms_pressed = dms - dms4;

	if (mspc_pressed + dms_pressed > 600) {

		if (dms_pressed > 600) {
			dms_pressed = 300;
		}

		mspc_pressed = 600 - dms_pressed;

		mspc_released = ms_per_char - mspc_pressed;
		dms_released = dms - dms_pressed;
	}

	while ((wc = *wstr++) != '\0') {

		keycode = wchar_to_keycode(wc, &shift, &ctrl, &alt);

		if (keycode > 0) {

			if (ctrl) {
				if (!ctrl_old) {
					sleep_ms(mspc4, dms4);
					send_key_down(VK_LCONTROL, 0);
				}
			}
			else if (ctrl_old) {
				send_key_up(VK_LCONTROL, 0);
			}

			if (alt) {
				if (!alt_old) {
					sleep_ms(mspc4, dms4);
					send_key_down(VK_RMENU, 0);
				}
			}
			else if (alt_old) {
				send_key_up(VK_RMENU, 0);
			}

			if (shift) {
				if (!shift_old) {
					sleep_ms(mspc4, dms4);
					send_key_down(VK_LSHIFT, 0);
				}
			}
			else if (shift_old) {
				send_key_up(VK_LSHIFT, 0);
			}

			sleep_ms(mspc_released, dms_released);

			send_key_down(keycode, 0);

			sleep_ms(mspc_pressed, dms_pressed);

			send_key_up(keycode, 0);

			ctrl_old = ctrl;
			alt_old = alt;
			shift_old = shift;
		}
	}
}