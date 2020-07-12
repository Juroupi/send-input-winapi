#ifndef _SEND_INPUT_H_
#define _SEND_INPUT_H_

#include <wchar.h>

/** Mouse button code
 */
enum {
	MOUSE_BUTTON_LEFT   = 0,
	MOUSE_BUTTON_RIGHT  = 1,
	MOUSE_BUTTON_MIDDLE = 2,
	MOUSE_BUTTON_X      = 3,
	MOUSE_BUTTON_X1     = MOUSE_BUTTON_X,
	MOUSE_BUTTON_X2     = MOUSE_BUTTON_X + 1,
};

/** Simulate mouse button press
 */
void send_button_down(int button);
void send_button_up(int button);

/** Simulate mouse move
 */
void send_mouse_move(int dx, int dy);

/** Simulate mouse scroll
 */
void send_mouse_wheel(int dx, int dy);

/** Simulate key press
 */
void send_key_down(int keycode, int extended);
void send_key_up(int keycode, int extended);

/** Convert char to virtual-key code
 */
int char_to_keycode(char c, int* shift, int* ctrl, int* alt);
int wchar_to_keycode(wchar_t wc, int* shift, int* ctrl, int* alt);

/** Simulate char
 */
void send_char(char c);
void send_wchar(wchar_t wc);

/** Convert words per minute to ms per char
 */
int wpm_to_mspc(float words_per_minute);

/** Simulate string
 * time per char = ms_per_char + random(-dms, dms)
 */
void send_string(const char* str, int ms_per_char, int dms);
void send_wstring(const wchar_t* wstr, int ms_per_char, int dms);

#endif