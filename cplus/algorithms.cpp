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


template<typename T, typename C>
void heap_sort(Array<T> in, C compare) {

    auto sift_down = [&](u64 root, u64 end) {

        while (2 * root + 1 <= end) {

            u64 child = 2 * root + 1;
            u64 swap  = root;

            if (compare(in[child], in[swap])) {
                swap = child;
            }
            
            if (child + 1 <= end && compare(in[child + 1], in[swap])) {
                swap = child + 1;
            }
            
            if (swap == root) break;

            in.swap(root, swap);
            root = swap;
        }
    };

    if (in.count < 2) return;
    
    u64 start = (in.count - 2) / 2 + 1;
    u64 end   = in.count - 1;
    
    while (start > 0) {
        start--;
        sift_down(start, end);
    }

    while (end > 0) {
        in.swap(end, 0);
        end--;
        sift_down(0, end);
    }
}




/* ==== Binary Search ==== */

template<typename T, typename C>
T* binary_search(Array<T> in, T key, C compare) {

    auto base  = in.data;
    auto count = in.count;
    
    while (count > 0) {
        
        auto test = &base[count / 2];

        s32 sign = compare(key, *test);

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
Array<T> binary_search_range(Array<T> in, T key, C compare) {
        
    auto start = binary_search(in, key, compare);
    if (!start) return {};
    
    u64 count = 1;
    
    while (start + count < in.data + in.count) {
        if (compare(key, start[count]) != 0) break;
        count++; 
    }

    while (start > in.data) {
        if (compare(key, *(start - 1)) != 0) break;
        start--; 
        count++;
    }

    return { start, count };
}

