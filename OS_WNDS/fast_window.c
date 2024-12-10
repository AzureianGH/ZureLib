#include "fast_window.h"

#include <windows.h>
#include "../zl_types.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
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

void __cdecl zl_qdraw_point(ZURELIB_WHDLE window, unsigned int x, unsigned int y, unsigned int color)
{
    HDC hdc = GetDC(window);
    SetPixel(hdc, x, y, color);
    ReleaseDC(window, hdc);
}

void __cdecl zl_qdraw_line(ZURELIB_WHDLE window, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color)
{
    HDC hdc = GetDC(window);
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    SelectObject(hdc, pen);
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
    DeleteObject(pen);
    ReleaseDC(window, hdc);
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
    TextOutA(hdc, x, y, str, strlen(str));
    ReleaseDC(window, hdc);
}

void __cdecl zl_qupdate_window(ZURELIB_WHDLE window)
{
    UpdateWindow(window);
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
#include <windows.h>

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

void __cdecl zl_qclear_window(ZURELIB_WHDLE window)
{
    HDC hdc = GetDC(window);
    RECT rect;
    GetClientRect(window, &rect);
    FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
    ReleaseDC(window, hdc);
}

double __cdecl zl_get_delta_time()
{
    return DeltaTime;
}

void __cdecl zl_qresize_window(ZURELIB_WHDLE window, unsigned int width, unsigned int height)
{
    SetWindowPos(window, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

#include "fast_io.h"

unsigned char __cdecl zl_qkey_down(ZL_KEYCODE key) {
    if (GetAsyncKeyState(key) & 0x8000)
    {
        return ZL_TRUE;
    }
    return ZL_FALSE;
}
