// src/platform_win32.c

#include <windows.h>
#include <stdint.h>
#include "../include/tinysdl.h"

static struct {
    HWND hwnd;
    HDC hdc;
    int should_close;

    TSDL_Framebuffer framebuffer;
    TSDL_Input input;

    BITMAPINFO bitmap_info;
    LARGE_INTEGER perf_freq;
    LARGE_INTEGER perf_start;
} win32;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            win32.should_close = 1;
            return 0;
        case WM_KEYDOWN:
            if (wParam < 256) win32.input.keys[wParam] = 1;
            return 0;
        case WM_KEYUP:
            if (wParam < 256) win32.input.keys[wParam] = 0;
            return 0;
        case WM_MOUSEMOVE:
            win32.input.mouse_x = LOWORD(lParam);
            win32.input.mouse_y = HIWORD(lParam);
            return 0;
        case WM_LBUTTONDOWN:
            win32.input.mouse_down = 1;
            return 0;
        case WM_LBUTTONUP:
            win32.input.mouse_down = 0;
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int tsdl_init(const char* title, int width, int height) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "TinySDLWindow";
    RegisterClass(&wc);

    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    win32.hwnd = CreateWindow("TinySDLWindow", title, WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              rect.right - rect.left, rect.bottom - rect.top,
                              NULL, NULL, GetModuleHandle(NULL), NULL);
    if (!win32.hwnd) return 0;

    ShowWindow(win32.hwnd, SW_SHOW);
    win32.hdc = GetDC(win32.hwnd);

    // Allocate framebuffer
    win32.framebuffer.width = width;
    win32.framebuffer.height = height;
    win32.framebuffer.pixels = (unsigned int*)malloc(width * height * sizeof(unsigned int));

    // Bitmap Info for StretchDIBits
    win32.bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    win32.bitmap_info.bmiHeader.biWidth = width;
    win32.bitmap_info.bmiHeader.biHeight = -height;  // Negative to flip vertically
    win32.bitmap_info.bmiHeader.biPlanes = 1;
    win32.bitmap_info.bmiHeader.biBitCount = 32;
    win32.bitmap_info.bmiHeader.biCompression = BI_RGB;

    // Init timer
    QueryPerformanceFrequency(&win32.perf_freq);
    QueryPerformanceCounter(&win32.perf_start);

    return 1;
}

void tsdl_shutdown() {
    free(win32.framebuffer.pixels);
    ReleaseDC(win32.hwnd, win32.hdc);
    DestroyWindow(win32.hwnd);
}

void tsdl_poll_events() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int tsdl_should_close() {
    return win32.should_close;
}

TSDL_Input* tsdl_get_input() {
    return &win32.input;
}

TSDL_Framebuffer* tsdl_get_framebuffer() {
    return &win32.framebuffer;
}

void tsdl_present() {
    StretchDIBits(win32.hdc, 0, 0, win32.framebuffer.width, win32.framebuffer.height,
                  0, 0, win32.framebuffer.width, win32.framebuffer.height,
                  win32.framebuffer.pixels, &win32.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
}

double tsdl_get_time() {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (double)(now.QuadPart - win32.perf_start.QuadPart) / win32.perf_freq.QuadPart;
}

void tsdl_sleep(int ms) {
    Sleep(ms);
}

void tsdl_draw_pixel(int x, int y, unsigned int color) {
    if (x < 0 || y < 0 || x >= win32.framebuffer.width || y >= win32.framebuffer.height) return;
    win32.framebuffer.pixels[y * win32.framebuffer.width + x] = color;
}

void tsdl_draw_rect(int x, int y, int w, int h, unsigned int color) {
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            tsdl_draw_pixel(x + i, y + j, color);
        }
    }
}

