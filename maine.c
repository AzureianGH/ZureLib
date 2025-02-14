#include "ZureLib/zurelib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void OnClose() {
    zl_deinit();
    exit(0);
}

void OnButtonClick() {
    zl_qmessage_box(NULL, "Button Clicked", "The button was clicked!", ZL_MB_OK);
}

int main() {
    zl_init(ZL_TRUE);
    zl_qset_console_visibility(ZL_TRUE);
    ZURELIB_WHDLE window = zl_qcreate_window("Simple GUI", 500, 500);
    if (!window) return 1;
    zl_register_on_close(&OnClose);
    
    ZL_BUTTON* button = zl_qcreate_button(0, 0, 75, 30, 12, "Click Me", &OnButtonClick, ZL_FALSE);
    ZL_BUTTON* button2 = zl_qcreate_button(0, 50, 75, 30, 12, "Click Me :3", &OnButtonClick, ZL_TRUE);
    while (!zl_qshould_window_close(window)) {
        zl_qdo_events(window);
        zl_qclear_window();
        zl_qdraw_button(window, button);
        zl_qdraw_button(window, button2);
        zl_qupdate_window(window);
    }

    zl_deinit();
    return 0;
}
