#ifndef _SORT_MERGE_HPP_
#define _SORT_MERGE_HPP_

template <typename C>
void merge_sort_merge(DWORD* data, size_t left, size_t mid, size_t right, C update) {
    size_t left_size = mid - left + 1;
    size_t right_size = right - mid;

    DWORD* left_array = new DWORD[left_size];
    DWORD* right_array = new DWORD[right_size];

    for(size_t i = 0; i < left_size; ++i)
        left_array[i] = data[left + i];
    for(size_t i = 0; i < right_size; ++i)
        right_array[i] = data[mid + i + 1];

    size_t i = 0, j = 0, k = left;
    while(i < left_size && j < right_size) {
        if(left_array[i] <= right_array[j]) {
            data[k] = left_array[i];
            ++i;
        }
        else {
            data[k] = right_array[j];
            ++j;
        }
        ++k;
    }
    while(i < left_size) {
        data[k] = left_array[i];
        ++i;
        ++k;
    }
    while(j < right_size) {
        data[k] = right_array[j];
        ++j;
        ++k;
    }
    delete[] left_array;
    delete[] right_array;

    update();
}

template <typename C>
void merge_sort_internal(DWORD* data, size_t left, size_t right, C update) {
    if(left < right) {
        size_t mid = left + (right - left) / 2;
        merge_sort_internal(data, left, mid, update);
        merge_sort_internal(data, mid + 1, right, update);
        merge_sort_merge(data, left, mid, right, update);
    }
}

template <typename C>
void merge_sort(DWORD* data, size_t width, size_t height, C update) {
    size_t max = width * height;

    merge_sort_internal(data, 0, max-1, update);
}

#endif
