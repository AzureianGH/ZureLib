#ifndef ZURELIB_OS_WNDS_FAST_WINDOW_H
#define ZURELIB_OS_WNDS_FAST_WINDOW_H

#include "../OS_CORE/calling_conv_ret.h"
#include "../zl_types.h"
#include <Windows.h>
#define ZURELIB_WHDLE void*
#define ZURELIB_WINDOW_VISIBLE 1
#define ZURELIB_WINDOW_HIDDEN 0

typedef struct {
    void* font;
    unsigned long long size;
    unsigned char encoding; // 0 = ANSI, 1 = UTF-8, 2 = UTF-16
    unsigned char style; // 0 = Regular, 1 = Bold, 2 = Italic, 3 = Bold Italic
} ZL_TTF_FONT;

typedef union {
    struct {
        ZL_BOOL resizable : 1; // 1 bit for resizable
        ZL_BOOL visible : 1; // 1 bit for visibility
        ZL_BOOL topmost : 1; // 1 bit for topmost
        ZL_BOOL minimized : 1; // 1 bit for minimized
        ZL_BOOL maximized : 1; // 1 bit for maximized
        ZL_BOOL fullscreen : 1; // 1 bit for fullscreen
        ZL_BOOL borderless : 1; // 1 bit for borderless
        ZL_BOOL _reserved : 1; // 1 bit reserved for future use
    };
    unsigned char flags;
} ZL_WINDOW_FLAGS;
/// @brief Quick-create a window
ZURELIB_API ZURELIB_RET(ZURELIB_WHDLE) __cdecl zl_qcreate_window(const char* title, unsigned int width, unsigned int height);

/// @brief Quick-destroy a window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qdestroy_window(ZURELIB_WHDLE window);

/// @brief Quick-set the window title
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qset_window_title(ZURELIB_WHDLE window, const char* title);

/// @brief Quick-set the window size
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qset_window_size(ZURELIB_WHDLE window, unsigned int width, unsigned int height);

/// @brief Quick-set the window position
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qset_window_position(ZURELIB_WHDLE window, unsigned int x, unsigned int y);

/// @brief Quick-set the window visibility
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qset_window_visibility(ZURELIB_WHDLE window, unsigned char visible);

/// @brief Quick-get the window visibility
ZURELIB_API ZURELIB_RET(unsigned char) __cdecl zl_qget_window_visibility(ZURELIB_WHDLE window);

/// @brief Quick-get the window size
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qget_window_size(ZURELIB_WHDLE window, unsigned int* width, unsigned int* height);

/// @brief Quick-get the window position
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qget_window_position(ZURELIB_WHDLE window, unsigned int* x, unsigned int* y);

/// @brief Quick-get the window title
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qget_window_title(ZURELIB_WHDLE window, char* title, unsigned int size);

/// @brief Draw a point on the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qdraw_point(unsigned int x, unsigned int y, unsigned int color);

/// @brief Draw a line on the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qdraw_line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);

/// @brief Draw a rectangle on the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qdraw_rect(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color);

/// @brief Draw a filled rectangle on the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qdraw_filled_rect(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color);

/// @brief Draw a circle on the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qdraw_circle(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int radius, unsigned int color);

/// @brief Draw a filled circle on the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qdraw_filled_circle(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int radius, unsigned int color);

/// @brief Draw a string on the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qdraw_string(ZURELIB_WHDLE window, unsigned int x, unsigned int y, const char* str, unsigned int color);

/// @brief Draw a string with a font on the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qdraw_string_font(ZURELIB_WHDLE window, unsigned int x, unsigned int y, const char* str, unsigned int color, ZL_TTF_FONT font);

/// @brief Update the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qupdate_window(ZURELIB_WHDLE window);

/// @brief Should the window close?
ZURELIB_API ZURELIB_RET(unsigned char) __cdecl zl_qshould_window_close(ZURELIB_WHDLE window);

/// @brief Do Events
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qdo_events(ZURELIB_WHDLE hdnle);

// @brief Clear the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qclear_window();

/// @brief Resize the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qresize_window(ZURELIB_WHDLE window, unsigned int width, unsigned int height);


/// @brief Register a function to be called when the window is clicked
ZURELIB_API ZURELIB_RET(void) __cdecl zl_register_on_click(void (*onClick)(void));

/// @brief Register a function to be called when the window is closed
ZURELIB_API ZURELIB_RET(void) __cdecl zl_register_on_close(void (*onClose)(void));

/// @brief Register a function to be called when the window is resized
ZURELIB_API ZURELIB_RET(void) __cdecl zl_register_on_resize(void (*onResize)(unsigned int width, unsigned int height));

/// @brief Send event to the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_send_event(ZURELIB_WHDLE window, unsigned int event);

/// @brief Terminate the window
ZURELIB_API ZURELIB_RET(void) __cdecl zl_terminate_window(ZURELIB_WHDLE window);

/// @brief Get Delta Time
ZURELIB_API ZURELIB_RET(double) __cdecl zl_get_delta_time();

/// @brief Set if the window should be resizable
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qset_window_resizable(ZURELIB_WHDLE window, unsigned char resizable);

/// @brief Set window flags
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qset_window_flags(ZURELIB_WHDLE window, ZL_WINDOW_FLAGS flags);

/// @brief Load a TrueType font from a file
ZURELIB_API ZURELIB_RET(ZL_TTF_FONT) __cdecl zl_load_ttf_font_file(const char* path);

/// @brief Load a TrueType font from memory
ZURELIB_API ZURELIB_RET(ZL_TTF_FONT) __cdecl zl_load_ttf_font_memory(const void* data, unsigned long long size);

/// @brief Free a TrueType font
ZURELIB_API ZURELIB_RET(void) __cdecl zl_free_ttf_font(ZL_TTF_FONT font);

/// @brief Set the window font
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qset_window_font(ZURELIB_WHDLE window, ZL_TTF_FONT font);

/// @brief Set the window font size
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qset_window_font_size(ZURELIB_WHDLE window, unsigned int size);

/// @brief Set the window icon (Bitmap format)
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qset_window_icon(ZURELIB_WHDLE window, HICON icon);

ZURELIB_API ZURELIB_RET(void) __cdecl zl_qset_console_visibility(unsigned char visible);

typedef struct {
    void (*onClick)(void);
    void (*onClose)(void);
    void (*onResize)(unsigned int width, unsigned int height);
} ZL_EventHandlers;

typedef enum {
    ZL_KEY_A = 0x41,
    ZL_KEY_B = 0x42,
    ZL_KEY_C = 0x43,
    ZL_KEY_D = 0x44,
    ZL_KEY_E = 0x45,
    ZL_KEY_F = 0x46,
    ZL_KEY_G = 0x47,
    ZL_KEY_H = 0x48,
    ZL_KEY_I = 0x49,
    ZL_KEY_J = 0x4A,
    ZL_KEY_K = 0x4B,
    ZL_KEY_L = 0x4C,
    ZL_KEY_M = 0x4D,
    ZL_KEY_N = 0x4E,
    ZL_KEY_O = 0x4F,
    ZL_KEY_P = 0x50,
    ZL_KEY_Q = 0x51,
    ZL_KEY_R = 0x52,
    ZL_KEY_S = 0x53,
    ZL_KEY_T = 0x54,
    ZL_KEY_U = 0x55,
    ZL_KEY_V = 0x56,
    ZL_KEY_W = 0x57,
    ZL_KEY_X = 0x58,
    ZL_KEY_Y = 0x59,
    ZL_KEY_Z = 0x5A,
    ZL_KEY_0 = 0x30,
    ZL_KEY_1 = 0x31,
    ZL_KEY_2 = 0x32,
    ZL_KEY_3 = 0x33,
    ZL_KEY_4 = 0x34,
    ZL_KEY_5 = 0x35,
    ZL_KEY_6 = 0x36,
    ZL_KEY_7 = 0x37,
    ZL_KEY_8 = 0x38,
    ZL_KEY_9 = 0x39,
    ZL_KEY_ESCAPE = 0x1B,
    ZL_KEY_ENTER = 0x0D,
    ZL_KEY_SPACE = 0x20,
    ZL_KEY_BACKSPACE = 0x08,
    ZL_KEY_TAB = 0x09,
    ZL_KEY_SHIFT = 0x10,
    ZL_KEY_CTRL = 0x11,
    ZL_KEY_ALT = 0x12,
    ZL_KEY_LEFT = 0x25,
    ZL_KEY_UP = 0x26,
    ZL_KEY_RIGHT = 0x27,
    ZL_KEY_DOWN = 0x28,
    ZL_KEY_F1 = 0x70,
    ZL_KEY_F2 = 0x71,
    ZL_KEY_F3 = 0x72,
    ZL_KEY_F4 = 0x73,
    ZL_KEY_F5 = 0x74,
    ZL_KEY_F6 = 0x75,
    ZL_KEY_F7 = 0x76,
    ZL_KEY_F8 = 0x77,
    ZL_KEY_F9 = 0x78,
    ZL_KEY_F10 = 0x79,
    ZL_KEY_F11 = 0x7A,
    ZL_KEY_F12 = 0x7B,
    ZL_KEY_PLUS = 0xBB,
    ZL_KEY_MINUS = 0xBD,
    ZL_KEY_KP_PLUS = 0x6B,
    ZL_KEY_KP_MINUS = 0x6D

} ZL_KEYCODE;

/// @brief Check for a key down
ZURELIB_API ZURELIB_RET(unsigned char) __cdecl zl_qkey_down(ZL_KEYCODE key);

#endif // ZURELIB_OS_WNDS_FAST_WINDOW_H