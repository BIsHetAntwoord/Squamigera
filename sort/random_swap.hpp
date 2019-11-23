#ifndef _SORT_RANDOM_SWAP_HPP_
#define _SORT_RANDOM_SWAP_HPP_

template <typename C>
void random_swap_sort(DWORD* data, size_t width, size_t height, C update) {
    std::minstd_rand engine_generator;
    std::random_device rd;
    engine_generator.seed(rd());

    size_t max = width * height;

    while(1) {
        size_t idx = engine_generator() % (max-1);

        if(data[idx] > data[idx+1])
            std::swap(data[idx], data[idx+1]);

        update();
    }
}

#endif // _SORT_RANDOM_SWAP_HPP_
