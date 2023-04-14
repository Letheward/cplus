template<typename T, typename C>
void bubble_sort(Array<T> in, C compare) {

    if (in.count < 2) return;

    while (1) {

        bool found = false;

        for (u64 i = 0; i < in.count - 1; i++) {

            auto a = in[i    ];
            auto b = in[i + 1];

            if (compare(a, b)) {
                in.data[i    ] = b;
                in.data[i + 1] = a;
                found = true;
            }
        }

        if (!found) break;
    }
}


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



