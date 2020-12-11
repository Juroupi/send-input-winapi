# send-input-winapi

Simulate keyboard/mouse inputs in c on windows

### Example

```c
#include "send_input.h"

int main() {
    
    send_mouse_move(-9999, 9999);

	send_mouse_click(MOUSE_BUTTON_LEFT, 100, 10);

	send_string("Hello", 0, wpm_to_mspc(45), 10);

    return 0;
}
```

