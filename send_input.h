#ifndef _SEND_INPUT_H_
#define _SEND_INPUT_H_

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
int char_to_keycode(int c, int* shift, int* ctrl, int* alt);
int wchar_to_keycode(int wc, int* shift, int* ctrl, int* alt);

/** Simulate char
 */
void send_char(char c);

/** Simulate string
 */
void send_string(char* str);

#endif