#define queue(T, c_array)   Queue<T> { c_array, count_of(c_array), 0, 0, 0 }

template<typename T>
struct Queue {

    T*  data;
    u64 size;
    u64 item_count;
    u64 start_index;
    u64 end_index;

    // this is a fixed size queue, so if you don't want to use calloc(), 
    // just ignore init() and deinit(), and supply your buffer to data and size.
    
    void init(u64 init_size = 32) {
        *this = {};
        data = (T*) calloc(init_size, sizeof(T));
        size = init_size;
    }

    void deinit() {
        free(data);
        *this = {};
    }

    void reset() {
        item_count  = 0;
        start_index = 0;
        end_index   = 0;
    }

    bool add(T item) {

        if (item_count > 0 && start_index == end_index) return false; // is full

        data[end_index] = item;
        end_index = (end_index + 1) % size;
        item_count++;

        return true;
    }

    Tuple2<T, bool> get() {

        if (item_count == 0 && start_index == end_index) return {}; // is empty

        auto out = data[start_index];
        start_index = (start_index + 1) % size;
        item_count--;

        return { out, true };
    }

    Tuple2<Array<T>, bool> to_array(Allocator allocator = Allocators::default_heap) {

        if (!item_count) return {};

        auto [out, ok] = Array<T>::alloc(item_count, allocator);
        if (!ok) return {};

        for (u64 i = 0; i < item_count; i++) {
            out.data[i] = data[(start_index + i) % size];
        }

        return { out, true };
    }


    /* ---- Iterators ---- */
    
    template<typename P>
    inline void for_each(P p) {
        for (u64 i = 0; i < item_count; i++) {
            u64 index = start_index + i;
            p(data[index % size]);
        }
    }
    
    template<typename P>
    inline void for_each_with_index(P p) {
        for (u64 i = 0; i < item_count; i++) {
            u64 index = start_index + i;
            p(data[index % size], i);
        }
    }
};


