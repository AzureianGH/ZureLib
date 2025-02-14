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
#include <stdint.h>
// Back buffer structure
typedef struct {
    HDC backBufferDC;
    HBITMAP backBufferBitmap;
    HBITMAP oldBitmap;
    int width;
    int height;
} BackBuffer;

BackBuffer g_backBuffer = { 0 };
ZL_BOOL MOUSETOGGLE = ZL_FALSE;
unsigned char scalingdpi = 1;
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

void __cdecl zl_set_dpi_scaling(unsigned char scaling)
{
    scalingdpi = scaling;
    if (scaling == 1)
    {
        SetProcessDPIAware();
    }
    else
    {
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);
    }
}

LPCWSTR __cdecl zl_qcstring_to_lpcwstr(const char* str) {
    wchar_t* wstr = (wchar_t*)malloc((strlen(str) + 1) * sizeof(wchar_t));
    mbstowcs(wstr, str, strlen(str) + 1);
    return wstr;
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

void __cdecl zl_qget_window_drawing_size(ZURELIB_WHDLE window, int* width, int* height) {
    RECT rect;
    GetClientRect(window, &rect);
    if (width != NULL) *width = rect.right - rect.left;
    if (height != NULL) *height = rect.bottom - rect.top;
}

void __cdecl zl_qget_window_size(ZURELIB_WHDLE window, int* width, int* height) {
    RECT rect;
    GetWindowRect(window, &rect);
    if (width != NULL) *width = rect.right - rect.left;
    if (height != NULL) *height = rect.bottom - rect.top;
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
    HBRUSH oldBrush = (HBRUSH)SelectObject(g_backBuffer.backBufferDC, GetStockObject(NULL_BRUSH));

    MoveToEx(g_backBuffer.backBufferDC, x1, y1, NULL);
    LineTo(g_backBuffer.backBufferDC, x2, y2);

    SelectObject(g_backBuffer.backBufferDC, oldPen);
    SelectObject(g_backBuffer.backBufferDC, oldBrush);

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
    TextOutW(g_backBuffer.backBufferDC, x, y, zl_qcstring_to_lpcwstr(str), (int)zl_qstrlen(str));
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

#include <process.h>

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
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
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

void __cdecl zl_qget_mouse_position(ZURELIB_WHDLE window, float* x, float* y) {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window, &point);
    *x = (float)point.x;
    *y = (float)point.y;
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

ZL_TTF_FONT __cdecl zl_load_ttf_font_file(const char* path, const char* fontName) {
    DWORD resultdw = AddFontResourceExW(zl_qcstring_to_lpcwstr(path), FR_PRIVATE, NULL);
    
    if (resultdw == 0) {
        printf("Failed to load font: %s\n", path);
        return (ZL_TTF_FONT){ 0 };
    }

    // Notify the system that a font has been added
    SendNotifyMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

    LOGFONT lf = {0};
    lf.lfHeight = -12;
    lf.lfWeight = FW_NORMAL;
    
    // Convert fontName from char* to wchar_t*
    MultiByteToWideChar(CP_UTF8, 0, fontName, -1, (LPWSTR)lf.lfFaceName, LF_FACESIZE);
    
    HFONT font = CreateFontIndirect(&lf);
    if (!font) {
        printf("Failed to create font: %s\n", fontName);
        return (ZL_TTF_FONT){ 0 };
    }

    return (ZL_TTF_FONT){ font, (void*)&font, zl_get_file_length(path) };
}


void __cdecl zl_free_ttf_font(ZL_TTF_FONT font, const char* path) {
    if (font.font) {
        DeleteObject((HFONT)font.font);
    }
    if (path) {
        RemoveFontResourceExW(zl_qcstring_to_lpcwstr(path), FR_PRIVATE, NULL);
        SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
    }
}


void __cdecl zl_qset_window_font(ZURELIB_WHDLE window, const char* fontName, int size) {
    wchar_t fontNameW[LF_FACESIZE];
    MultiByteToWideChar(CP_UTF8, 0, fontName, -1, fontNameW, LF_FACESIZE);

    HFONT hFont = CreateFontW(
        -MulDiv(size, GetDeviceCaps(GetDC(window), LOGPIXELSY), 72), 0, 0, 0, FW_DONTCARE, 
        FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, 
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, fontNameW);
    
    SendMessage(window, WM_SETFONT, (WPARAM)hFont, TRUE);
}


void __cdecl zl_qset_window_font_size(ZURELIB_WHDLE window, unsigned int size) {
    HFONT hFont = CreateFont(
        -MulDiv(size, GetDeviceCaps(GetDC(window), LOGPIXELSY), 72), 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH, NULL);
    SendMessage(window, WM_SETFONT, (WPARAM)hFont, TRUE);
}

// Draw a string using a custom font on the back buffer
void __cdecl zl_qdraw_string_font(ZURELIB_WHDLE window, int x, int y, int size, const char* str, int color, const char* fontName) {
    wchar_t fontNameW[LF_FACESIZE];
    MultiByteToWideChar(CP_UTF8, 0, fontName, -1, fontNameW, LF_FACESIZE);

    HFONT hFont = CreateFontW(
        -MulDiv(size, GetDeviceCaps(g_backBuffer.backBufferDC, LOGPIXELSY), 72), 0, 0, 0, 
        FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, 
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, fontNameW);

    HFONT oldFont = (HFONT)SelectObject(g_backBuffer.backBufferDC, hFont);
    SetTextColor(g_backBuffer.backBufferDC, color);
    SetBkMode(g_backBuffer.backBufferDC, TRANSPARENT);
    
    TextOutA(g_backBuffer.backBufferDC, x, y, str, (int)strlen(str));

    SelectObject(g_backBuffer.backBufferDC, oldFont);
    DeleteObject(hFont);
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

ZL_MESSAGE_BOX_RETURN __cdecl zl_qmessage_box(ZURELIB_WHDLE window, const char* title, const char* message, ZL_MESSAGE_BOX_TYPE type) {
    UINT flags = 0;
    switch (type) {
        case ZL_MB_OK:
            flags = MB_OK;
            break;
        case ZL_MB_OK_CANCEL:
            flags = MB_OKCANCEL;
            break;
        case ZL_MB_YES_NO:
            flags = MB_YESNO;
            break;
        case ZL_MB_YES_NO_CANCEL:
            flags = MB_YESNOCANCEL;
            break;
    }
    int ret = MessageBoxA(window, message, title, flags);
    switch (ret) {
        case IDOK:
            return ZL_MB_OK_RETURN;
        case IDCANCEL:
            return ZL_MB_CANCEL_RETURN;
        case IDYES:
            return ZL_MB_YES_RETURN;
        case IDNO:
            return ZL_MB_NO_RETURN;
        default:
            return ZL_MB_CANCEL_RETURN;
    }
}

// zl_qdraw_button
void __cdecl zl_qdraw_button(ZURELIB_WHDLE window, ZL_BUTTON* button) {
    if (!button || !button->visible) {
        return;
    }

    // Get window size and clamp button coords
    int wndWidth = 0, wndHeight = 0;
    zl_qget_window_drawing_size(window, &wndWidth, &wndHeight);
    int x1 = button->x < 0 ? 0 : button->x;
    int y1 = button->y < 0 ? 0 : button->y;
    int x2 = (button->x + button->width) > wndWidth ? wndWidth : (button->x + button->width);
    int y2 = (button->y + button->height) > wndHeight ? wndHeight : (button->y + button->height);

    int drawWidth = x2 - x1;
    int drawHeight = y2 - y1;
    if (drawWidth <= 0 || drawHeight <= 0) {
        return;
    }

    // Draw base
    zl_qdraw_filled_rect(window, x1, y1, drawWidth, drawHeight, RGB(125, 125, 125));

    // Edges
    zl_qdraw_line(x1, y1, x1 + drawWidth - 1, y1, RGB(255, 255, 255));
    zl_qdraw_line(x1, y1, x1, y1 + drawHeight - 1, RGB(255, 255, 255));
    zl_qdraw_line(x1 + drawWidth - 1, y1, x1 + drawWidth - 1, y1 + drawHeight - 1, RGB(128, 128, 128));
    zl_qdraw_line(x1, y1 + drawHeight - 1, x1 + drawWidth - 1, y1 + drawHeight - 1, RGB(128, 128, 128));

    // Inner
    zl_qdraw_filled_rect(window, x1 + 2, y1 + 2, drawWidth - 4, drawHeight - 4, RGB(224, 224, 224));

    // Center text
    unsigned int textWidth = (unsigned int)zl_qstrlen(button->text) * 8;
    unsigned int textHeight = 16;
    unsigned int textX = x1 + (drawWidth / 2) - (textWidth / 2);
    unsigned int textY = y1 + (drawHeight / 2) - (textHeight / 2);

    POINT cursor;
    GetCursorPos(&cursor);
    ScreenToClient(window, &cursor);

    // Cursor detection only if cursor is within clamped region
    if (cursor.x >= x1 && cursor.x < x1 + (LONG)drawWidth &&
        cursor.y >= y1 && cursor.y < y1 + (LONG)drawHeight)
    {
        zl_qdraw_filled_rect(window, x1 + 2, y1 + 2, drawWidth - 4, drawHeight - 4, RGB(208, 208, 208));

        // Pressed effect
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && MOUSETOGGLE == ZL_FALSE) {
            zl_qdraw_filled_rect(window, x1 + 2, y1 + 2, drawWidth - 4, drawHeight - 4, RGB(160, 160, 160));

            zl_qdraw_line(x1, y1, x1 + drawWidth - 1, y1, RGB(128, 128, 128));
            zl_qdraw_line(x1, y1, x1, y1 + drawHeight - 1, RGB(128, 128, 128));
            zl_qdraw_line(x1 + drawWidth - 1, y1, x1 + drawWidth - 1, y1 + drawHeight - 1, RGB(255, 255, 255));
            zl_qdraw_line(x1, y1 + drawHeight - 1, x1 + drawWidth - 1, y1 + drawHeight - 1, RGB(255, 255, 255));

            if (button->onClick) {
                button->onClick();
            }
            MOUSETOGGLE = ZL_TRUE;
        } else {
            MOUSETOGGLE = ZL_FALSE;
        }
    }

    zl_qdraw_string_font(window, textX, textY, button->text_size, button->text, RGB(0, 0, 0), NULL);
}


// zl_qcreate_button
ZL_BUTTON* __cdecl zl_qcreate_button(int x, int y, int width, int height, int text_size, const char* text, void (*onClick)(void), ZL_BOOL auto_width) {
    ZL_BUTTON* button = (ZL_BUTTON*)malloc(sizeof(ZL_BUTTON));
    if (!button) {
        return NULL;
    }

    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    zl_qstrcpy(button->text, text);
    button->onClick = onClick;
    button->text_size = text_size;
    button->width_auto = auto_width;
    if (auto_width) {
        button->width = (zl_qget_string_width(text) * button->text_size ) + 16;
    }
    button->visible = ZL_TRUE;
    return button;
}

void __cdecl zl_show_tooltip(ZURELIB_WHDLE window, const char* text, int x, int y) {
    // Calculate the width and height of the tooltip
    int textWidth = zl_qstrlen(text) * 6;
    int textHeight = 16;
    int padding = 4;
    int tooltipWidth = textWidth + padding * 2;
    int tooltipHeight = textHeight + padding * 2;

    // Draw the tooltip background with a light yellow color
    zl_qdraw_filled_rect(window, x, y, tooltipWidth, tooltipHeight, RGB(255, 255, 225));

    // Draw the tooltip border with a dark gray color
    zl_qdraw_rect(window, x, y, tooltipWidth, tooltipHeight, RGB(128, 128, 128));

    // Draw the text inside the tooltip
    zl_qdraw_string_font(window, x + padding, y + padding, 12, text, RGB(0, 0, 0), NULL);
}

void __cdecl zl_show_mouse_tooltip(ZURELIB_WHDLE window, const char* text) {
    POINT cursor;
    GetCursorPos(&cursor);
    ScreenToClient(window, &cursor);
    zl_show_tooltip(window, text, cursor.x + 16, cursor.y + 16);
}

void __cdecl zl_dock_item(ZURELIB_WHDLE window, ZURELIB_WHDLE item, ZL_DOCK_LOCATION location)
{
    ZL_DEFAULT_ELEMENT transform = { 0 };
    memcpy(&transform, item, sizeof(ZL_DEFAULT_ELEMENT)); // get the item's position and size
    switch (location)
    {
    case ZL_DOCK_TOP:
        transform.x = 0;
        transform.y = 0;
        zl_qget_window_drawing_size(window, &transform.width, NULL);
        break;
    case ZL_DOCK_BOTTOM:
        zl_qget_window_drawing_size(window, &transform.width, &transform.height);
        transform.x = 0;
        transform.y = transform.height - transform.height;
        break;
    case ZL_DOCK_LEFT:
        transform.x = 0;
        transform.y = 0;
        zl_qget_window_drawing_size(window, &transform.width, &transform.height);
        break;
    case ZL_DOCK_RIGHT:
        zl_qget_window_drawing_size(window, &transform.width, &transform.height);
        transform.x = transform.width - transform.width;
        transform.y = 0;
        break;
    case ZL_DOCK_FILL:  
        zl_qget_window_drawing_size(window, &transform.width, &transform.height);
        transform.x = 0;
        transform.y = 0;
        break;
    }
    transform.docked = ZL_TRUE;
    memcpy(item, &transform, sizeof(ZL_DEFAULT_ELEMENT)); // set the item's position and size
}

static HANDLE g_drawThread = NULL;
static volatile BOOL g_running = TRUE;
//drawingthread func ptr
void (*drawingthread)(void) = NULL;
unsigned __stdcall DrawThread(void* param) {
    ZURELIB_WHDLE window = (ZURELIB_WHDLE)param;
    while (g_running) {
        if (window) {
            // Render your scene
            drawingthread();
            zl_qupdate_window(window);
        }
        
    }
    return 0;
}

void zl_start_dedicated_render_thread(ZURELIB_WHDLE window, void (*drawing)(void)) {
    g_running = TRUE;
    drawingthread = drawing;
    g_drawThread = (HANDLE)_beginthreadex(NULL, 0, DrawThread, window, 0, NULL);
}

void zl_stop_dedicated_render_thread() {
    g_running = FALSE;
    if (g_drawThread) {
        WaitForSingleObject(g_drawThread, INFINITE);
        CloseHandle(g_drawThread);
        g_drawThread = NULL;
    }
}

//disable -Werror=maybe-uninitialized
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

//label
ZL_LABEL* __cdecl zl_qcreate_label(int x, int y, int width, int height, int text_size, const char* text, ZL_BOOL auto_width, ZL_BOOL no_background) {
    ZL_LABEL* label = (ZL_LABEL*)malloc(sizeof(ZL_LABEL));
    if (!label) {
        return NULL;
    }

    label->x = x;
    label->y = y;
    label->width = width;
    label->height = height;
    label->text_size = text_size;
    label->width_auto = auto_width;
    label->visible = ZL_TRUE;
    label->docked = ZL_FALSE;
    label->text = (char*)malloc(zl_qstrlen(text) + 1);
    label->no_background = no_background;
    zl_qstrcpy(label->text, text);
    return label;
}

#pragma GCC diagnostic pop

//zl_qdraw_label
void __cdecl zl_qdraw_label(ZURELIB_WHDLE window, ZL_LABEL* label) {
    if (!label || !label->visible) {
        return;
    }

    // Get window size and clamp label coords
    int wndWidth = 0, wndHeight = 0;
    zl_qget_window_drawing_size(window, &wndWidth, &wndHeight);
    int x1 = label->x < 0 ? 0 : label->x;
    int y1 = label->y < 0 ? 0 : label->y;
    int x2 = (label->x + label->width) > wndWidth ? wndWidth : (label->x + label->width);
    int y2 = (label->y + label->height) > wndHeight ? wndHeight : (label->y + label->height);

    int drawWidth = x2 - x1;
    int drawHeight = y2 - y1;
    if (drawWidth <= 0 || drawHeight <= 0) {
        return;
    }
    if (!label->no_background) {
        // Draw base
        zl_qdraw_filled_rect(window, x1, y1, drawWidth, drawHeight, RGB(255, 255, 255));

        // Edges
        zl_qdraw_line(x1, y1, x1 + drawWidth - 1, y1, RGB(128, 128, 128));
        zl_qdraw_line(x1, y1, x1, y1 + drawHeight - 1, RGB(128, 128, 128));
        zl_qdraw_line(x1 + drawWidth - 1, y1, x1 + drawWidth - 1, y1 + drawHeight - 1, RGB(255, 255, 255));
        zl_qdraw_line(x1, y1 + drawHeight - 1, x1 + drawWidth - 1, y1 + drawHeight - 1, RGB(255, 255, 255));

        // Inner
        zl_qdraw_filled_rect(window, x1 + 2, y1 + 2, drawWidth - 4, drawHeight - 4, RGB(224, 224, 224));
    }

    // Center text
    unsigned int textWidth = (unsigned int)zl_qstrlen(label->text) * 8;
    unsigned int textHeight = 16;
    unsigned int textX = x1 + (drawWidth / 2) - (textWidth / 2);
    unsigned int textY = y1 + (drawHeight / 2) - (textHeight / 2);

    zl_qdraw_string_font(window, textX, textY, label->text_size, label->text, RGB(0, 0, 0), NULL);
}

//zl_create_textbox
ZL_TEXTBOX* __cdecl zl_qcreate_textbox(int x, int y, int width, int height, int text_size, const char* text, ZL_BOOL readonly, ZL_BOOL has_cursor, ZL_BOOL multiline, ZL_BOOL password, ZL_BOOL no_background)
{
    ZL_TEXTBOX* textbox = (ZL_TEXTBOX*)malloc(sizeof(ZL_TEXTBOX));
    if (!textbox) {
        return NULL;
    }

    textbox->x = x;
    textbox->y = y;
    textbox->width = width;
    textbox->height = height;
    textbox->text_size = text_size;
    textbox->readonly = readonly;
    textbox->has_cursor = has_cursor;
    textbox->multiline = multiline;
    textbox->password = password;
    textbox->no_background = no_background;
    textbox->visible = ZL_TRUE;
    textbox->docked = ZL_FALSE;
    textbox->text = (char*)malloc(zl_qstrlen(text) + 1);
    zl_qstrcpy(textbox->text, text);
    return textbox;
}
void zl_qhandle_textbox_input(unsigned int wParam);

static ZL_TEXTBOX* g_activeTextbox = NULL;

static void draw_multiline_text(ZURELIB_WHDLE window, ZL_TEXTBOX* textbox, int startX, int startY) {
    char* temp = _strdup(textbox->text);
    char* line = strtok(temp, "\n");
    int lineOffsetY = 0;
    while (line) {
        zl_qdraw_string_font(window, startX, startY + lineOffsetY, textbox->text_size, line, RGB(0,0,0), NULL);
        lineOffsetY += 16; 
        line = strtok(NULL, "\n");
    }
    free(temp);
}

void zl_qdraw_textbox(ZURELIB_WHDLE window, ZL_TEXTBOX* textbox) {
    if (!textbox || !textbox->visible) return;

    // Get window sizing and clamp coords
    int wndWidth = 0, wndHeight = 0;
    zl_qget_window_drawing_size(window, &wndWidth, &wndHeight);
    int x1 = textbox->x < 0 ? 0 : textbox->x;
    int y1 = textbox->y < 0 ? 0 : textbox->y;
    int x2 = (textbox->x + textbox->width) > wndWidth ? wndWidth : (textbox->x + textbox->width);
    int y2 = (textbox->y + textbox->height) > wndHeight ? wndHeight : (textbox->y + textbox->height);
    int drawWidth = x2 - x1;
    int drawHeight = y2 - y1;

    if (drawWidth <= 0 || drawHeight <= 0) return;

    // Check click logic
    POINT cursor; 
    GetCursorPos(&cursor);
    ScreenToClient(window, &cursor);
    if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
        if (cursor.x >= x1 && cursor.x < x1 + drawWidth && cursor.y >= y1 && cursor.y < y1 + drawHeight) {
            g_activeTextbox = textbox;
        } else if (g_activeTextbox == textbox) {
            g_activeTextbox = NULL;
        }
    }

    // Draw background
    if (!textbox->no_background) {
        zl_qdraw_filled_rect(window, x1, y1, drawWidth, drawHeight, RGB(255,255,255));
        zl_qdraw_line(x1, y1, x1+drawWidth-1, y1, RGB(128,128,128));
        zl_qdraw_line(x1, y1, x1, y1+drawHeight-1, RGB(128,128,128));
        zl_qdraw_line(x1+drawWidth-1, y1, x1+drawWidth-1, y1+drawHeight-1, RGB(255,255,255));
        zl_qdraw_line(x1, y1+drawHeight-1, x1+drawWidth-1, y1+drawHeight-1, RGB(255,255,255));
        zl_qdraw_filled_rect(window, x1+2, y1+2, drawWidth-4, drawHeight-4, RGB(224,224,224));
    }

    // Draw text
    if (textbox->password) {
        int len = (int)zl_qstrlen(textbox->text);
        char* masked = (char*)malloc(len+1);
        memset(masked, '*', len);
        masked[len] = '\0';
        zl_qdraw_string_font(window, x1+4, y1+4, textbox->text_size, masked, RGB(0,0,0), NULL);
        free(masked);
    } else if (textbox->multiline) {
        draw_multiline_text(window, textbox, x1+4, y1+4);
    } else {
        zl_qdraw_string_font(window, x1+4, y1+4, textbox->text_size, textbox->text, RGB(0,0,0), NULL);
    }

    // Draw cursor if active
    if (g_activeTextbox == textbox && textbox->has_cursor) {
        // Simple cursor at the end of the last line
        unsigned int len = (unsigned int)zl_qstrlen(textbox->text);
        unsigned int cursorX = x1 + 4 + (len * 8);
        int lineCount = 1;
        for (unsigned int i = 0; i < len; i++) {
            if (textbox->text[i] == '\n') lineCount++;
        }
        int cursorY = y1 + 4 + (lineCount-1)*16;
        zl_qdraw_line(cursorX, cursorY, cursorX, cursorY+16, RGB(0,0,0));
    }

    // Outline if active
    if (g_activeTextbox == textbox) {
        zl_qdraw_rect(window, x1, y1, drawWidth, drawHeight, RGB(0,0,0));
    }
}

void zl_qhandle_textbox_input(unsigned int wParam) {
    if (!g_activeTextbox || g_activeTextbox->readonly) return;
    if (wParam == VK_BACK) {
        unsigned int len = zl_qstrlen(g_activeTextbox->text);
        if (len > 0) g_activeTextbox->text[len-1] = '\0';
    } else if (wParam == VK_RETURN && g_activeTextbox->multiline) {
        unsigned int len = zl_qstrlen(g_activeTextbox->text);
        if (len < 254) {
            g_activeTextbox->text[len] = '\n';
            g_activeTextbox->text[len+1] = '\0';
        }
    } else if (wParam >= 32 && wParam <= 126) {
        unsigned int len = zl_qstrlen(g_activeTextbox->text);
        if (len < 255) {
            g_activeTextbox->text[len] = (char)wParam;
            g_activeTextbox->text[len+1] = '\0';
        }
    }
}
