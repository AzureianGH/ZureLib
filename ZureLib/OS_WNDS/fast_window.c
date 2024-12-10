#include "fast_window.h"

#include <windows.h>
#include "../zl_types.h"
#include "../OS_CORE/zl_common.h"
#include "fast_io.h"
#include "../OS_CORE/fast_str.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    HDC backBufferDC;
    HBITMAP backBufferBitmap;
    HBITMAP oldBitmap;
    int width;
    int height;
} BackBuffer;

BackBuffer g_backBuffer = { 0 };

void InitializeBackBuffer(HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);

    HDC hdc = GetDC(hwnd);
    g_backBuffer.width = rect.right - rect.left;
    g_backBuffer.height = rect.bottom - rect.top;

    g_backBuffer.backBufferDC = CreateCompatibleDC(hdc);
    g_backBuffer.backBufferBitmap = CreateCompatibleBitmap(hdc, g_backBuffer.width, g_backBuffer.height);
    g_backBuffer.oldBitmap = (HBITMAP)SelectObject(g_backBuffer.backBufferDC, g_backBuffer.backBufferBitmap);

    ReleaseDC(hwnd, hdc);
}

void DestroyBackBuffer() {
    SelectObject(g_backBuffer.backBufferDC, g_backBuffer.oldBitmap);
    DeleteObject(g_backBuffer.backBufferBitmap);
    DeleteDC(g_backBuffer.backBufferDC);
}
    

static ZL_EventHandlers globalEventHandlers = { NULL, NULL, NULL };

void zl_register_on_click(void (*callback)(void)) {
    globalEventHandlers.onClick = callback;
}

void zl_register_on_close(void (*callback)(void)) {
    globalEventHandlers.onClose = callback;
}

//on resize
void zl_register_on_resize(void (*callback)(unsigned int width, unsigned int height)) {
    globalEventHandlers.onResize = callback;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            if (globalEventHandlers.onClose) {
                globalEventHandlers.onClose();  // Trigger close handler
            }
            PostQuitMessage(0);
            break;

        case WM_LBUTTONDOWN:
            if (globalEventHandlers.onClick) {
                globalEventHandlers.onClick();  // Trigger click handler
            }
            break;

        case WM_SIZE:
            if (globalEventHandlers.onResize) {
                unsigned int width = LOWORD(lParam);
                unsigned int height = HIWORD(lParam);
                globalEventHandlers.onResize(width, height);  // Trigger resize handler
            }
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Create a window
ZURELIB_WHDLE __cdecl zl_qcreate_window(const char* title, unsigned int width, unsigned int height) {
    static int classCounter = 0;
    char className[64];
    snprintf(className, sizeof(className), "ZureLibWindowClass%d", classCounter++);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;  // Use our custom WndProc
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
        WS_OVERLAPPEDWINDOW,  // Allow resizing
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, wc.hInstance, NULL
    );

    if (!hwnd) {
        MessageBox(NULL, "Window creation failed!", "Error", MB_OK | MB_ICONERROR);
        return NULL;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    return hwnd;  // Return the window handle
}


void __cdecl zl_qdestroy_window(ZURELIB_WHDLE window)
{
    DestroyWindow(window);
}

void __cdecl zl_qset_window_title(ZURELIB_WHDLE window, const char* title)
{
    SetWindowTextA(window, title);
}

void __cdecl zl_qset_window_size(ZURELIB_WHDLE window, unsigned int width, unsigned int height)
{
    SetWindowPos(window, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void __cdecl zl_qset_window_position(ZURELIB_WHDLE window, unsigned int x, unsigned int y)
{
    SetWindowPos(window, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void __cdecl zl_qset_window_visibility(ZURELIB_WHDLE window, unsigned char visible)
{
    ShowWindow(window, visible ? SW_SHOW : SW_HIDE);
}

unsigned char __cdecl zl_qget_window_visibility(ZURELIB_WHDLE window)
{
    return IsWindowVisible(window) ? ZURELIB_WINDOW_VISIBLE : ZURELIB_WINDOW_HIDDEN;
}

void __cdecl zl_qget_window_size(ZURELIB_WHDLE window, unsigned int* width, unsigned int* height)
{
    RECT rect;
    GetWindowRect(window, &rect);
    *width = rect.right - rect.left;
    *height = rect.bottom - rect.top;
}

void __cdecl zl_qget_window_position(ZURELIB_WHDLE window, unsigned int* x, unsigned int* y)
{
    RECT rect;
    GetWindowRect(window, &rect);
    *x = rect.left;
    *y = rect.top;
}

void __cdecl zl_qget_window_title(ZURELIB_WHDLE window, char* title, unsigned int size)
{
    GetWindowTextA(window, title, size);
}

void __cdecl zl_qdraw_point(unsigned int x, unsigned int y, unsigned int color) {
    SetPixel(g_backBuffer.backBufferDC, x, y, color);
}

void __cdecl zl_qdraw_line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color) {
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    SelectObject(g_backBuffer.backBufferDC, pen);
    MoveToEx(g_backBuffer.backBufferDC, x1, y1, NULL);
    LineTo(g_backBuffer.backBufferDC, x2, y2);
    DeleteObject(pen);
}

void __cdecl zl_qdraw_rect(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color)
{
    HDC hdc = GetDC(window);
    RECT rect = { x, y, x + width, y + height };
    HBRUSH brush = CreateSolidBrush(color);
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
    ReleaseDC(window, hdc);
}

void __cdecl zl_qdraw_filled_rect(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color)
{
    HDC hdc = GetDC(window);
    RECT rect = { x, y, x + width, y + height };
    HBRUSH brush = CreateSolidBrush(color);
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
    ReleaseDC(window, hdc);
}

void __cdecl zl_qdraw_circle(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int radius, unsigned int color)
{
    HDC hdc = GetDC(window);
    HBRUSH brush = CreateSolidBrush(color);
    SelectObject(hdc, brush);
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
    DeleteObject(brush);
    ReleaseDC(window, hdc);
}

void __cdecl zl_qdraw_filled_circle(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int radius, unsigned int color)
{
    HDC hdc = GetDC(window);
    HBRUSH brush = CreateSolidBrush(color);
    SelectObject(hdc, brush);
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
    DeleteObject(brush);
    ReleaseDC(window, hdc);
}

void __cdecl zl_qdraw_string(ZURELIB_WHDLE window, unsigned int x, unsigned int y, const char* str, unsigned int color)
{
    HDC hdc = GetDC(window);
    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);
    TextOutA(hdc, x, y, str, zl_qstrlen(str));
    ReleaseDC(window, hdc);
}

void __cdecl zl_qupdate_window(ZURELIB_WHDLE window) {
    HDC hdc = GetDC(window);
    BitBlt(hdc, 0, 0, g_backBuffer.width, g_backBuffer.height, g_backBuffer.backBufferDC, 0, 0, SRCCOPY);
    ReleaseDC(window, hdc);
}

unsigned char __cdecl zl_qshould_window_close(ZURELIB_WHDLE window)
{
    //if window is destroyed, return true
    if (!IsWindow(window))
    {
        return ZL_TRUE;
    }
    MSG msg;
    if (PeekMessageA(&msg, window, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
        if (msg.message == WM_CLOSE)
        {
            return ZL_TRUE;
        }
    }
    return ZL_FALSE;
}
double DeltaTime = 0.0;


void __cdecl zl_qdo_events(ZURELIB_WHDLE hdnle)
{
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

    // If window is destroyed, return
    if (!IsWindow(hdnle)) {
        return;
    }

    MSG msg;
    while (PeekMessageA(&msg, hdnle, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

void __cdecl zl_send_event(ZURELIB_WHDLE window, unsigned int event)
{
    PostMessageA(window, event, 0, 0);
}

void __cdecl zl_terminate_window(ZURELIB_WHDLE window)
{
    PostMessageA(window, WM_CLOSE, 0, 0);
    // or try using DestroyWindow directly
    DestroyWindow(window);
}

void __cdecl zl_qclear_window() {
    RECT rect = { 0, 0, g_backBuffer.width, g_backBuffer.height };
    FillRect(g_backBuffer.backBufferDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));
}

double __cdecl zl_get_delta_time()
{
    return DeltaTime;
}

void __cdecl zl_qresize_window(ZURELIB_WHDLE window, unsigned int width, unsigned int height)
{
    SetWindowPos(window, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void __cdecl zl_qset_window_resizable(ZURELIB_WHDLE window, unsigned char resizable)
{
    SetWindowLong(window, GWL_STYLE, resizable ? WS_OVERLAPPEDWINDOW : WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME);
}

void __cdecl zl_qset_window_flags(ZURELIB_WHDLE window, ZL_WINDOW_FLAGS flags)
{
    if (flags.resizable) {
        SetWindowLong(window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
    }
    else {
        SetWindowLong(window, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME);
    }

    if (flags.visible) {
        ShowWindow(window, SW_SHOW);
    }
    else {
        ShowWindow(window, SW_HIDE);
    }

    if (flags.topmost) {
        SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
    else {
        SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    if (flags.minimized) {
        ShowWindow(window, SW_MINIMIZE);
    }
    else {
        ShowWindow(window, SW_RESTORE);
    }

    if (flags.maximized) {
        ShowWindow(window, SW_MAXIMIZE);
    }
    else {
        ShowWindow(window, SW_RESTORE);
    }

    if (flags.fullscreen) {
        SetWindowLong(window, GWL_STYLE, WS_POPUP);
        SetWindowPos(window, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
    }
    else {
        SetWindowLong(window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    if (flags.borderless) {
        SetWindowLong(window, GWL_STYLE, WS_POPUP);
    }
    else {
        SetWindowLong(window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
    }
    
}

unsigned char __cdecl zl_qkey_down(ZL_KEYCODE key) {
    if (GetAsyncKeyState(key) & 0x8000)
    {
        return ZL_TRUE;
    }
    return ZL_FALSE;
}
// version, fontRevision, checkSumAdjustment, magicNumber, flags, unitsPerEm, created, modified, xMin, yMin, xMax, yMax, macStyle, lowestRecPPEM, fontDirectionHint, indexToLocFormat, glyphDataFormat
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
    // Check if data is null
    if (!data || size == 0) {
        return (ZL_TTF_FONT){ 0 };
    }

    DWORD numFonts = 0;

    // Load the font into GDI
    HANDLE fontHandle = AddFontMemResourceEx((void*)data, (DWORD)size, NULL, &numFonts);
    if (!fontHandle || numFonts == 0) {
        printf("Failed to load TTF font.\n");
        return (ZL_TTF_FONT){ 0 };
    }

    // Create an HFONT object
    HFONT hFont = CreateFont(
        24,             // Height (you can change this dynamically)
        0,              // Width (auto-calculated)
        0,              // Escapement
        0,              // Orientation
        FW_NORMAL,      // Weight
        FALSE,          // Italic
        FALSE,          // Underline
        FALSE,          // Strikeout
        DEFAULT_CHARSET,// Charset
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        "CustomFont"    // Font name
    );

    if (!hFont) {
        printf("Failed to create HFONT.\n");
        RemoveFontMemResourceEx(fontHandle);
        return (ZL_TTF_FONT){ 0 };
    }

    // Return the font structure
    ZL_TTF_FONT font = { 0 };
    font.font = hFont;
    font.size = size;
    font.encoding = 0; // Can be extended for UTF-8/UTF-16
    font.style = 0;

    return font;
}

ZL_TTF_FONT __cdecl zl_load_ttf_font_file(const char* path)
{
    unsigned long long size;
    ZL_FILE_HANDLE file = zl_load_file(path, &size);
    if (!file) {
        return (ZL_TTF_FONT){ 0 };
    }

    ZL_TTF_FONT font = zl_local_parse_ttf(file, size);
    return font;
}

ZL_TTF_FONT __cdecl zl_load_ttf_font_memory(const void* data, unsigned long long size)
{
    ZL_TTF_FONT font = zl_local_parse_ttf(data, size);
    return font;
}

void __cdecl zl_free_ttf_font(ZL_TTF_FONT font) {
    if (font.font) {
        // Delete the HFONT object
        DeleteObject((HFONT)font.font);
    }
}

void __cdecl zl_qset_window_font(ZURELIB_WHDLE window, ZL_TTF_FONT font)
{
    // Set the font
    SendMessage(window, WM_SETFONT, (WPARAM)font.font, TRUE);
}

void __cdecl zl_qset_window_font_size(ZURELIB_WHDLE window, unsigned int size)
{
    // Set the font size
    SendMessage(window, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, size));
}

//draw font string (zl_qdraw_string_font)
void __cdecl zl_qdraw_string_font(ZURELIB_WHDLE window, unsigned int x, unsigned int y, const char* str, unsigned int color, ZL_TTF_FONT font) {
    if (!font.font) {
        printf("Invalid font.\n");
        return;
    }

    HDC hdc = GetDC(window);
    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);

    // Select the HFONT into the device context
    HFONT oldFont = (HFONT)SelectObject(hdc, (HFONT)font.font);
    TextOutA(hdc, x, y, str, (int)zl_qstrlen(str));

    // Restore the previous font
    SelectObject(hdc, oldFont);
    ReleaseDC(window, hdc);
}

void __cdecl zl_qset_window_icon(ZURELIB_WHDLE window, HICON icon)
{
    SendMessageW(window, WM_SETICON, ICON_SMALL, (LPARAM)icon);
    SendMessageW(window, WM_SETICON, ICON_BIG, (LPARAM)icon);
}
void __cdecl zl_qset_console_visibility(unsigned char visible)
{
    HWND console = GetConsoleWindow();
    
    if (!visible) {
        FreeConsole();
    } else {
        AllocConsole();
    }

    
}