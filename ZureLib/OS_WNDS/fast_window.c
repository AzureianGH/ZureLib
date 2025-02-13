#include "fast_window.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <windows.h>
#include "../zl_types.h"
#include "../OS_CORE/zl_common.h"
#include "fast_io.h"
#include "../OS_CORE/fast_str.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

// Back buffer structure
typedef struct {
    HDC backBufferDC;
    HBITMAP backBufferBitmap;
    HBITMAP oldBitmap;
    int width;
    int height;
} BackBuffer;

BackBuffer g_backBuffer = { 0 };

// Initialize back buffer
void InitializeBackBuffer(HWND hwnd) {
    if (!hwnd) return;
    RECT rect;
    GetClientRect(hwnd, &rect);
    int newWidth = rect.right - rect.left;
    int newHeight = rect.bottom - rect.top;
    if (newWidth <= 0 || newHeight <= 0) return; // Avoid zero-size

    HDC hdc = GetDC(hwnd);

    // Clean up old buffer first, if needed
    if (g_backBuffer.backBufferDC) {
        SelectObject(g_backBuffer.backBufferDC, g_backBuffer.oldBitmap);
        DeleteObject(g_backBuffer.backBufferBitmap);
        DeleteDC(g_backBuffer.backBufferDC);
    }

    g_backBuffer.width = newWidth;
    g_backBuffer.height = newHeight;

    g_backBuffer.backBufferDC = CreateCompatibleDC(hdc);
    g_backBuffer.backBufferBitmap = CreateCompatibleBitmap(hdc, g_backBuffer.width, g_backBuffer.height);
    g_backBuffer.oldBitmap = (HBITMAP)SelectObject(g_backBuffer.backBufferDC, g_backBuffer.backBufferBitmap);

    ReleaseDC(hwnd, hdc);

    // Clear the buffer once
    RECT fillRect = {0, 0, g_backBuffer.width, g_backBuffer.height};
    HBRUSH brush = (HBRUSH)(COLOR_WINDOW + 1);
    FillRect(g_backBuffer.backBufferDC, &fillRect, brush);
}

// Destroy back buffer
void DestroyBackBuffer() {
    if (g_backBuffer.backBufferDC) {
        SelectObject(g_backBuffer.backBufferDC, g_backBuffer.oldBitmap);
        DeleteObject(g_backBuffer.backBufferBitmap);
        DeleteDC(g_backBuffer.backBufferDC);
        g_backBuffer.backBufferDC = NULL;
    }
}

static ZL_EventHandlers globalEventHandlers = { NULL, NULL, NULL };

void zl_register_on_click(void (*callback)(void)) {
    globalEventHandlers.onClick = callback;
}

void zl_register_on_close(void (*callback)(void)) {
    globalEventHandlers.onClose = callback;
}

void zl_register_on_resize(void (*callback)(unsigned int width, unsigned int height)) {
    globalEventHandlers.onResize = callback;
}

// Main window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            if (globalEventHandlers.onClose) {
                globalEventHandlers.onClose();
            }
            PostQuitMessage(0);
            break;

        case WM_LBUTTONDOWN:
            if (globalEventHandlers.onClick) {
                globalEventHandlers.onClick();
            }
            break;

        case WM_SIZE:
            if (globalEventHandlers.onResize) {
                unsigned int width = LOWORD(lParam);
                unsigned int height = HIWORD(lParam);
                globalEventHandlers.onResize(width, height);
            }
            // Re-initialize back buffer whenever the window is resized
            DestroyBackBuffer();
            InitializeBackBuffer(hwnd);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

ZURELIB_WHDLE __cdecl zl_qcreate_window(const char* title, unsigned int width, unsigned int height) {
    static int classCounter = 0;
    char className[64];
    snprintf(className, sizeof(className), "ZureLibWindowClass%d", classCounter++);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = className;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window registration failed!", "Error", MB_OK | MB_ICONERROR);
        return NULL;
    }

    HWND hwnd = CreateWindowEx(
        0,
        className,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, wc.hInstance, NULL
    );

    if (!hwnd) {
        MessageBox(NULL, "Window creation failed!", "Error", MB_OK | MB_ICONERROR);
        return NULL;
    }

    // Initialize backbuffer here
    InitializeBackBuffer(hwnd);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    return hwnd;
}

void __cdecl zl_qdestroy_window(ZURELIB_WHDLE window) {
    DestroyWindow(window);
}

void __cdecl zl_qset_window_title(ZURELIB_WHDLE window, const char* title) {
    SetWindowTextA(window, title);
}

void __cdecl zl_qset_window_size(ZURELIB_WHDLE window, unsigned int width, unsigned int height) {
    SetWindowPos(window, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void __cdecl zl_qset_window_position(ZURELIB_WHDLE window, unsigned int x, unsigned int y) {
    SetWindowPos(window, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void __cdecl zl_qset_window_visibility(ZURELIB_WHDLE window, unsigned char visible) {
    ShowWindow(window, visible ? SW_SHOW : SW_HIDE);
}

unsigned char __cdecl zl_qget_window_visibility(ZURELIB_WHDLE window) {
    return IsWindowVisible(window) ? ZURELIB_WINDOW_VISIBLE : ZURELIB_WINDOW_HIDDEN;
}

void __cdecl zl_qget_window_size(ZURELIB_WHDLE window, unsigned int* width, unsigned int* height) {
    RECT rect;
    GetWindowRect(window, &rect);
    *width = rect.right - rect.left;
    *height = rect.bottom - rect.top;
}

void __cdecl zl_qget_window_position(ZURELIB_WHDLE window, unsigned int* x, unsigned int* y) {
    RECT rect;
    GetWindowRect(window, &rect);
    *x = rect.left;
    *y = rect.top;
}

void __cdecl zl_qget_window_title(ZURELIB_WHDLE window, char* title, unsigned int size) {
    GetWindowTextA(window, title, size);
}

// Draw a single pixel on the back buffer
void __cdecl zl_qdraw_point(unsigned int x, unsigned int y, unsigned int color) {
    SetPixel(g_backBuffer.backBufferDC, x, y, color);
}

// Draw a line on the back buffer
void __cdecl zl_qdraw_line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color) {
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HPEN oldPen = (HPEN)SelectObject(g_backBuffer.backBufferDC, pen);
    MoveToEx(g_backBuffer.backBufferDC, x1, y1, NULL);
    LineTo(g_backBuffer.backBufferDC, x2, y2);
    SelectObject(g_backBuffer.backBufferDC, oldPen);
    DeleteObject(pen);
}

// Draw rect on the back buffer
void __cdecl zl_qdraw_rect(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color) {
    HBRUSH brush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(g_backBuffer.backBufferDC, brush);
    RECT rect = { (LONG)x, (LONG)y, (LONG)(x + width), (LONG)(y + height) };
    FrameRect(g_backBuffer.backBufferDC, &rect, brush);
    SelectObject(g_backBuffer.backBufferDC, oldBrush);
    DeleteObject(brush);
}

// Draw filled rect on the back buffer
void __cdecl zl_qdraw_filled_rect(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color) {
    HBRUSH brush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(g_backBuffer.backBufferDC, brush);
    RECT rect = { (LONG)x, (LONG)y, (LONG)(x + w), (LONG)(y + h) };
    FillRect(g_backBuffer.backBufferDC, &rect, brush);
    SelectObject(g_backBuffer.backBufferDC, oldBrush);
    DeleteObject(brush);
}

// Draw a circle (outline) on the back buffer
void __cdecl zl_qdraw_circle(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int radius, unsigned int color) {
    HBRUSH brush = CreateSolidBrush(RGB(255,255,255)); 
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(g_backBuffer.backBufferDC, brush);
    HPEN oldPen = (HPEN)SelectObject(g_backBuffer.backBufferDC, pen);
    Arc(g_backBuffer.backBufferDC, x - radius, y - radius, x + radius, y + radius, 0, 0, 0, 0);
    SelectObject(g_backBuffer.backBufferDC, oldBrush);
    SelectObject(g_backBuffer.backBufferDC, oldPen);
    DeleteObject(brush);
    DeleteObject(pen);
}

// Draw a filled circle on the back buffer
void __cdecl zl_qdraw_filled_circle(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int radius, unsigned int color) {
    HBRUSH brush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(g_backBuffer.backBufferDC, brush);
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HPEN oldPen = (HPEN)SelectObject(g_backBuffer.backBufferDC, pen);

    Ellipse(g_backBuffer.backBufferDC, x - radius, y - radius, x + radius, y + radius);

    SelectObject(g_backBuffer.backBufferDC, oldBrush);
    SelectObject(g_backBuffer.backBufferDC, oldPen);
    DeleteObject(pen);
    DeleteObject(brush);
}

// Draw a string on the back buffer
void __cdecl zl_qdraw_string(ZURELIB_WHDLE window, unsigned int x, unsigned int y, const char* str, unsigned int color) {
    SetTextColor(g_backBuffer.backBufferDC, color);
    SetBkMode(g_backBuffer.backBufferDC, TRANSPARENT);
    TextOutA(g_backBuffer.backBufferDC, x, y, str, (int)zl_qstrlen(str));
}

// Present back buffer to the window
void __cdecl zl_qupdate_window(ZURELIB_WHDLE window) {
    if (!g_backBuffer.backBufferDC) return;
    HDC hdc = GetDC(window);
    BitBlt(hdc, 0, 0, g_backBuffer.width, g_backBuffer.height, g_backBuffer.backBufferDC, 0, 0, SRCCOPY);
    ReleaseDC(window, hdc);
}

unsigned char __cdecl zl_qshould_window_close(ZURELIB_WHDLE window) {
    if (!IsWindow(window)) {
        return ZL_TRUE;
    }
    MSG msg;
    if (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
        if (msg.message == WM_CLOSE) {
            return ZL_TRUE;
        }
    }
    return ZL_FALSE;
}

double DeltaTime = 0.0;

void __cdecl zl_qdo_events(ZURELIB_WHDLE hdnle) {
    static LARGE_INTEGER frequency;
    static LARGE_INTEGER lastTime;
    LARGE_INTEGER currentTime;

    if (frequency.QuadPart == 0) {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&lastTime);
    }

    QueryPerformanceCounter(&currentTime);
    DeltaTime = (double)(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
    lastTime = currentTime;

    if (!IsWindow(hdnle)) {
        return;
    }

    MSG msg;
    while (PeekMessageA(&msg, hdnle, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

void __cdecl zl_send_event(ZURELIB_WHDLE window, unsigned int event) {
    PostMessageA(window, event, 0, 0);
}

void __cdecl zl_terminate_window(ZURELIB_WHDLE window) {
    PostMessageA(window, WM_CLOSE, 0, 0);
    DestroyWindow(window);
}

void __cdecl zl_qclear_window() {
    if (g_backBuffer.backBufferDC) {
        RECT rect = { 0, 0, g_backBuffer.width, g_backBuffer.height };
        FillRect(g_backBuffer.backBufferDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));
    }
}

double __cdecl zl_get_delta_time() {
    return DeltaTime;
}

void __cdecl zl_qresize_window(ZURELIB_WHDLE window, unsigned int width, unsigned int height) {
    SetWindowPos(window, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void __cdecl zl_qset_window_resizable(ZURELIB_WHDLE window, unsigned char resizable) {
    LONG style = GetWindowLong(window, GWL_STYLE);
    if (resizable)
        style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
    else
        style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
    SetWindowLong(window, GWL_STYLE, style);
    SetWindowPos(window, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE);
}

void __cdecl zl_qset_window_flags(ZURELIB_WHDLE window, ZL_WINDOW_FLAGS flags) {
    if (flags.resizable) {
        SetWindowLong(window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
    } else {
        SetWindowLong(window, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME);
    }

    if (flags.visible) {
        ShowWindow(window, SW_SHOW);
    } else {
        ShowWindow(window, SW_HIDE);
    }

    if (flags.topmost) {
        SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    } else {
        SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    if (flags.minimized) {
        ShowWindow(window, SW_MINIMIZE);
    } else {
        ShowWindow(window, SW_RESTORE);
    }

    if (flags.maximized) {
        ShowWindow(window, SW_MAXIMIZE);
    } else {
        ShowWindow(window, SW_RESTORE);
    }

    if (flags.fullscreen) {
        SetWindowLong(window, GWL_STYLE, WS_POPUP);
        SetWindowPos(window, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN),
                     GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
    } else {
        SetWindowLong(window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    if (flags.borderless) {
        SetWindowLong(window, GWL_STYLE, WS_POPUP);
    } else {
        SetWindowLong(window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
    }
}

unsigned char __cdecl zl_qkey_down(ZL_KEYCODE key) {
    return (GetAsyncKeyState(key) & 0x8000) ? ZL_TRUE : ZL_FALSE;
}

unsigned char __cdecl zl_qmouse_button_down(ZL_MOUSE_BUTTON button) {
    switch (button) {
        case ZL_MOUSE_LEFT:
            return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? ZL_TRUE : ZL_FALSE;
        case ZL_MOUSE_RIGHT:
            return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? ZL_TRUE : ZL_FALSE;
        case ZL_MOUSE_MIDDLE:
            return (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? ZL_TRUE : ZL_FALSE;
        default:
            return ZL_FALSE;
    }
}

unsigned int __cdecl zl_qmouse_x() {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(GetForegroundWindow(), &point);
    return point.x;
}

unsigned int __cdecl zl_qmouse_y() {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(GetForegroundWindow(), &point);
    return point.y;
}

// TTF font structures and loading
typedef struct {
    unsigned int version;
    unsigned int fontRevision;
    unsigned int checkSumAdjustment;
    unsigned int magicNumber;
    unsigned short flags;
    unsigned short unitsPerEm;
    unsigned long long created;
    unsigned long long modified;
    int xMin;
    int yMin;
    int xMax;
    int yMax;
    unsigned short macStyle;
    unsigned short lowestRecPPEM;
    short fontDirectionHint;
    short indexToLocFormat;
    short glyphDataFormat;
} ttf_header_font_zl;

ZL_TTF_FONT __cdecl zl_local_parse_ttf(const void* data, unsigned long long size) {
    if (!data || size == 0) {
        return (ZL_TTF_FONT){ 0 };
    }

    DWORD numFonts = 0;
    HANDLE fontHandle = AddFontMemResourceEx((void*)data, (DWORD)size, NULL, &numFonts);
    if (!fontHandle || numFonts == 0) {
        printf("Failed to load TTF font.\n");
        return (ZL_TTF_FONT){ 0 };
    }

    HFONT hFont = CreateFont(
        24, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, "CustomFont"
    );

    if (!hFont) {
        printf("Failed to create HFONT.\n");
        RemoveFontMemResourceEx(fontHandle);
        return (ZL_TTF_FONT){ 0 };
    }

    ZL_TTF_FONT font = { 0 };
    font.font = hFont;
    font.size = size;
    font.encoding = 0;
    font.style = 0;
    return font;
}

ZL_TTF_FONT __cdecl zl_load_ttf_font_file(const char* path) {
    unsigned long long size;
    ZL_FILE_HANDLE file = zl_load_file(path, &size);
    if (!file) {
        return (ZL_TTF_FONT){ 0 };
    }
    return zl_local_parse_ttf(file, size);
}

ZL_TTF_FONT __cdecl zl_load_ttf_font_memory(const void* data, unsigned long long size) {
    return zl_local_parse_ttf(data, size);
}

void __cdecl zl_free_ttf_font(ZL_TTF_FONT font) {
    if (font.font) {
        DeleteObject((HFONT)font.font);
    }
}

void __cdecl zl_qset_window_font(ZURELIB_WHDLE window, ZL_TTF_FONT font) {
    SendMessage(window, WM_SETFONT, (WPARAM)font.font, TRUE);
}

void __cdecl zl_qset_window_font_size(ZURELIB_WHDLE window, unsigned int size) {
    SendMessage(window, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, size));
}

// Draw a string using a custom font on the back buffer
void __cdecl zl_qdraw_string_font(ZURELIB_WHDLE window, unsigned int x, unsigned int y,
                                  const char* str, unsigned int color, ZL_TTF_FONT font) {
    if (!font.font) {
        printf("Invalid font.\n");
        return;
    }
    SetTextColor(g_backBuffer.backBufferDC, color);
    SetBkMode(g_backBuffer.backBufferDC, TRANSPARENT);

    HFONT oldFont = (HFONT)SelectObject(g_backBuffer.backBufferDC, (HFONT)font.font);
    TextOutA(g_backBuffer.backBufferDC, x, y, str, (int)zl_qstrlen(str));
    SelectObject(g_backBuffer.backBufferDC, oldFont);
}

// Set window icon
void __cdecl zl_qset_window_icon(ZURELIB_WHDLE window, HICON icon) {
    SendMessageW(window, WM_SETICON, ICON_SMALL, (LPARAM)icon);
    SendMessageW(window, WM_SETICON, ICON_BIG, (LPARAM)icon);
}
HICON __cdecl zl_load_from_image_file(const char* filePath) {
    return (HICON)LoadImage(NULL, filePath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
}

// Show/hide console
void __cdecl zl_qset_console_visibility(unsigned char visible) {
    if (!visible) {
        FreeConsole();
    } else {
        AllocConsole();
    }
}
