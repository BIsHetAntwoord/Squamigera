#ifndef _SORT_INSERTION_HPP_
#define _SORT_INSERTION_HPP_

template <typename C>
void insertion_sort(DWORD* data, size_t width, size_t height, C update) {
    size_t max = width * height;

    for(size_t i = 0; i < max; ++i) {
        size_t j = i;

        while(j > 0 && data[j] < data[j-1]) {
            std::swap(data[j], data[j-1]);

            --j;
            update();
        }
    }
}

#endif
