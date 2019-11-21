#include <windows.h>
#include <iostream>
#include <random>
#include <algorithm>

const size_t NUM_SORT = (1920 * 1080);
const size_t SLEEP_TIMEOUT = 10 * 1000;
const size_t SORT_LOOKAHEAD = 128;

template <typename T>
void modify_buffer(DWORD* data, size_t width, size_t height, T& random_engine) {
    size_t max = width * height;
    for(size_t i = 0; i < NUM_SORT; ++i) {
        size_t p1 = random_engine() % (max-1);
        size_t p2 = p1 + random_engine() % SORT_LOOKAHEAD;
        if(p2 >= max)
            p2 = max-1;

        if(p1 > p2)
            std::swap(p1, p2);

        if(data[p1] > data[p2])
            std::swap(data[p1], data[p2]);
    }
}

int main() {
    std::minstd_rand engine;
    std::random_device seed_device;
    engine.seed(seed_device());

    Sleep(SLEEP_TIMEOUT);

    HDC desktop_device = GetDC(0);

    RECT window_rect;
    GetClientRect(WindowFromDC(desktop_device), &window_rect);
    size_t width = window_rect.right - window_rect.left;
    size_t height = window_rect.bottom - window_rect.top;

    while(1) {
        HBITMAP screen_bitmap = CreateCompatibleBitmap(desktop_device, width, height);
        HDC capture = CreateCompatibleDC(desktop_device);
        HGDIOBJ obj = SelectObject(capture, screen_bitmap);
        BitBlt(capture, 0, 0, width, height, desktop_device, 0, 0, SRCCOPY | CAPTUREBLT);

        BITMAPINFOHEADER bitmap_header = {sizeof(BITMAPINFOHEADER)};
        bitmap_header.biWidth = width;
        bitmap_header.biHeight = height;
        bitmap_header.biPlanes = 1;
        bitmap_header.biBitCount = 32;

        DWORD size = width * height;
        DWORD* bytes = new DWORD[size];

        DWORD result = GetDIBits(desktop_device, screen_bitmap, 0, height, bytes, (BITMAPINFO*)&bitmap_header, DIB_RGB_COLORS);
        if(result > 0) {
            modify_buffer(bytes, width, height, engine);

            SetDIBits(desktop_device, screen_bitmap, 0, height, bytes, (BITMAPINFO*)&bitmap_header, DIB_RGB_COLORS);
            BitBlt(desktop_device, 0, 0, width, height, capture, 0, 0, SRCCOPY | CAPTUREBLT);
        }
        SelectObject(capture, obj);

        DeleteObject(screen_bitmap);
        DeleteDC(capture);

        delete[] bytes;
    }

    return 0;
}
