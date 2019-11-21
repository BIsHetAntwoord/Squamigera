#include <windows.h>
#include <iostream>
#include <random>
#include <algorithm>

const size_t NUM_SORT = (1920 * 1080);
const size_t SLEEP_TIMEOUT = 10 * 1000;
const size_t SORT_LOOKAHEAD = 128;

struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct HSV {
    uint8_t a;
    uint8_t v;
    uint8_t s;
    uint8_t h;
};

HSV rgb_to_hsv(RGB* rgb) {
    uint8_t min;
    uint8_t max;
    HSV result;

    min = rgb->r < rgb->g ? (rgb->r < rgb->b ? rgb->r : rgb->b) : (rgb->g < rgb->b ? rgb->g : rgb->b);
    max = rgb->r > rgb->g ? (rgb->r > rgb->b ? rgb->r : rgb->b) : (rgb->g > rgb->b ? rgb->g : rgb->b);

    result.v = max;
    if(result.v == 0) {
        result.h = 0;
        result.s = 0;
        return result;
    }

    result.s = 255 * long(max - min) / result.v;
    if(result.s == 0) {
        result.h = 0;
        return result;
    }

    if(max == rgb->r)
        result.h = 0 + 43 * (rgb->g - rgb->b) / (max - min);
    else if(max == rgb->g)
        result.h = 85 + 43 * (rgb->b - rgb->r) / (max - min);
    else
        result.h = 171 + 43 * (rgb->r - rgb->g) / (max - min);
    return result;
}

bool color_greater(DWORD p1, DWORD p2) {
    RGB* p1_rgb = (RGB*)&p1;
    RGB* p2_rgb = (RGB*)&p2;

    HSV p1_hsv = rgb_to_hsv(p1_rgb);
    HSV p2_hsv = rgb_to_hsv(p2_rgb);

    uint32_t p1_hsv_bits = *(uint32_t*)(&p1_hsv);
    uint32_t p2_hsv_bits = *(uint32_t*)(&p2_hsv);

    return p1_hsv_bits > p2_hsv_bits;
}

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

    COLORREF* data_buffer = new COLORREF[width];

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

    delete[] data_buffer;

    return 0;
}
