#include <windows.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <cstring>

#include "sort/insertion.hpp"
#include "sort/merge.hpp"
#include "sort/quick.hpp"
#include "sort/random_swap.hpp"

const size_t SLEEP_TIMEOUT = 3 * 1000;

enum class SortAlgorithm {
    INSERTION_SORT,
    MERGE_SORT,
    QUICK_SORT,
    RANDOM_SWAP_SORT
};

const char* const ALGORITHM_NAMES[] = {
    "insertion",
    "merge",
    "quick",
    "random_swap"
};

const size_t NUM_ALGORITHMS = 4;

size_t SORT_SPEEDS[] = {
    2 << 20, //Insertion
    2 << 5, //Merge
    2 << 3, //Quick
    2 << 18 //Random swap
};

template <typename C>
void run_sorter(SortAlgorithm algorithm, DWORD* data, size_t width, size_t height, C update) {
    switch(algorithm) {
        case SortAlgorithm::INSERTION_SORT:
            insertion_sort(data, width, height, update);
            break;
        case SortAlgorithm::MERGE_SORT:
            merge_sort(data, width, height, update);
            break;
        case SortAlgorithm::QUICK_SORT:
            quick_sort(data, width, height, update);
            break;
        case SortAlgorithm::RANDOM_SWAP_SORT:
            random_swap_sort(data, width, height, update);
            break;
    }
}

SortAlgorithm convert_algorithm(const char* alg_name) {
    for(size_t i = 0; i < NUM_ALGORITHMS; ++i) {
        if(std::strcmp(alg_name, ALGORITHM_NAMES[i]) == 0)
            return (SortAlgorithm)i;
    }
    return SortAlgorithm::RANDOM_SWAP_SORT;
}

int main(int argc, char* argv[]) {
    SortAlgorithm algorithm;

    if(argc >= 2) {
        algorithm = convert_algorithm(argv[1]);
    }
    else {
        std::random_device rd;
        std::minstd_rand random(rd());

        algorithm = (SortAlgorithm)(random() % NUM_ALGORITHMS);
    }
    Sleep(SLEEP_TIMEOUT);

    HDC desktop_device = GetDC(0);

    RECT window_rect;
    GetClientRect(WindowFromDC(desktop_device), &window_rect);
    size_t width = window_rect.right - window_rect.left;
    size_t height = window_rect.bottom - window_rect.top;

    bool active = true;

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
            size_t sort_offset = 0;
            auto update_screen = [&]() {
                ++sort_offset;
                if(sort_offset >= SORT_SPEEDS[(size_t)algorithm])
                {
                    SetDIBits(desktop_device, screen_bitmap, 0, height, bytes, (BITMAPINFO*)&bitmap_header, DIB_RGB_COLORS);
                    BitBlt(desktop_device, 0, 0, width, height, capture, 0, 0, SRCCOPY | CAPTUREBLT);
                    sort_offset = 0;
                }
            };

            while(active) {
                run_sorter(algorithm, bytes, width, height, update_screen);
                update_screen();
            }
        }
        SelectObject(capture, obj);

        DeleteObject(screen_bitmap);
        DeleteDC(capture);

        delete[] bytes;
    }

    return 0;
}
