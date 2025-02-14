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
ZL_LABEL* label;
ZL_TEXTBOX* textbox;
ZURELIB_WHDLE window;
void drawingthread(void)
{
    
    zl_qclear_window();
    zl_qdraw_label(window, label);
    zl_qdraw_textbox(window, textbox);
}

int main() {
    zl_init(ZL_TRUE);
    zl_qset_console_visibility(ZL_TRUE);
    window = zl_qcreate_window("Simple GUI", 500, 500);
    if (!window) return 1;
    zl_register_on_close(&OnClose);
    label = zl_qcreate_label(0, 0, 100, 30, 12, "Hello World", ZL_TRUE, ZL_FALSE);
    textbox = zl_qcreate_textbox(0, 50, 100, 30, 12, "Hello World", ZL_FALSE, ZL_TRUE, ZL_FALSE, ZL_FALSE, ZL_FALSE);
    

    //turn off dpi awareness
    zl_set_dpi_scaling(ZL_FALSE);
    zl_start_dedicated_render_thread(window, &drawingthread);
    while (!zl_qshould_window_close(window)) {
        zl_qdo_events(window);
    }

    zl_deinit();
    return 0;
}
