/* ==== Sort ==== */

template<typename T, typename C>
void insertion_sort(Array<T> in, C compare) {

    if (in.count < 2) return;

    for (u64 i = 1; i < in.count; i++) {

        auto key = in[i];

        u64 j = i;
        while (j > 0 && compare(in[j - 1], key)) {
            in.data[j] = in[j - 1];
            j--;
        }

        in.data[j] = key;
    }
}


template<typename T, typename C>
void quick_sort_helper(Array<T> in, s64 start, s64 end, C compare) {

    if (start < 0 || start >= end) return;

    auto pivot = in[end];

    s64 i = start;
    for (s64 j = start; j < end; j++) {
        if (compare(pivot, in[j])) {
            in.swap(i, j);
            i++;
        }
    }

    in.swap(i, end);

    quick_sort_helper(in, start, i - 1, compare);
    quick_sort_helper(in, i + 1, end,   compare);
}

template<typename T, typename C>
void quick_sort(Array<T> in, C compare) {
    if (in.count < 2) return;
    quick_sort_helper(in, 0, (s64) in.count - 1, compare);
}




/* ==== Binary Search ==== */

template<typename T, typename C>
T* binary_search(Array<T> in, T key, C c) {

    auto base  = in.data;
    auto count = in.count;
    
    while (count > 0) {
        
        auto test = &base[count / 2];

        s32 sign = c(key, *test);

        if (sign < 0) {
            count /= 2;
        } else if (sign > 0) {
            base = &test[1];
            count -= (count / 2) + 1;
        } else {
            return test;
        }
    }

    return NULL;
}

template<typename T, typename C>
Array<T> binary_search_range(Array<T> in, T key, C c) {
        
    auto start = binary_search(in, key, c);
    if (!start) return {};
    
    u64 count = 1;
    
    while (start + count < in.data + in.count) {
        if (c(key, start[count]) != 0) break;
        count++; 
    }

    while (start > in.data) {
        if (c(key, *(start - 1)) != 0) break;
        start--; 
        count++;
    }

    return { start, count };
}

