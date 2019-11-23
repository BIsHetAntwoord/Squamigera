#ifndef _SORT_QUICK_HPP_
#define _SORT_QUICK_HPP_

inline size_t quick_sort_part(DWORD* data, size_t left, size_t right) {
    DWORD v = data[left];

    size_t i = left;
    size_t j = right + 1;

    do
    {
        do {
            ++i;
        } while(data[i] < v && i <= j);

        do {
            --j;
        } while(v < data[j]);

        if(i < j) {
            std::swap(data[i], data[j]);
        }
    } while(i < j);

    data[left] = data[j];
    data[j] = v;

    return j;
}

template <typename C>
void quick_sort_internal(DWORD* data, size_t left, size_t right, C update) {
    if(left < right) {
        size_t pivot = quick_sort_part(data, left, right);
        quick_sort_internal(data, left, pivot-1, update);
        quick_sort_internal(data, pivot+1, right, update);
    }
    update();
}

template <typename C>
void quick_sort(DWORD* data, size_t width, size_t height, C update) {
    size_t max = width * height;
    quick_sort_internal(data, 0, max-1, update);
}

#endif
