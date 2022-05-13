#ifndef _SEND_INPUT_H_
#define _SEND_INPUT_H_

#include <wchar.h>

/** Suspend execution
 */
void sleep_ms(int ms, int dms);

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

/** Simulate mouse button click
 */
void send_button_down(int button);
void send_button_up(int button);

void send_mouse_click(int button, int click_time_ms, int dms);
void send_mouse_dblclick(int button, int click_time_ms, int dms);

/** Simulate mouse move
 */
void send_mouse_move(int dx, int dy);

/** Simulate mouse scroll
 */
void send_mouse_wheel(int dx, int dy);

/** Simulate key press
 */
void send_key_down(int keycode, int extended, int send_scancode);
void send_key_up(int keycode, int extended, int send_scancode);

void send_key_press(int keycode, int extended, int send_scancode, int press_time_ms, int dms);

/** Convert char to virtual-key code
 */
int char_to_keycode(char c, int* shift, int* ctrl, int* alt);
int wchar_to_keycode(wchar_t wc, int* shift, int* ctrl, int* alt);

/** Write char
 */
void send_char(char c, int send_scancodes, int press_time_ms, int dms);
void send_wchar(wchar_t wc, int send_scancodes, int press_time_ms, int dms);

/** Write unicode char
 */
void send_unicode(int codepoint);

/** Convert words per minute to ms per char
 */
int wpm_to_mspc(float words_per_minute);

/** Write string
 * time per char = ms_per_char + random(-dms, dms)
 */
void send_string(const char* str, int send_scancodes, int ms_per_char, int dms);
void send_wstring(const wchar_t* wstr, int send_scancodes, int ms_per_char, int dms);

/** Get mouse position
 * returns 0 if it fails, an other value otherwise
 */
int get_mouse_pos(int* x, int* y);

/** Get window title bar height
 */
int get_title_bar_height();

/** Get window title bar buttons width
 * returns 0 if it fails, an other value otherwise
 */
int get_foreground_window_box(int* x, int* y, int* w, int* h);

#endif