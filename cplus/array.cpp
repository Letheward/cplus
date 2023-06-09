/* ==== Macros ==== */

#define count_of(array)      (sizeof(array) / sizeof(array[0]))
#define array(T, c_array)    Array<T> {c_array, count_of(c_array)}




/* ==== Array ==== */

template<typename T>
struct Array {

    T*  data;
    u64 count;

    inline T operator [] (u64 index) {
        return data[index];
    }

    inline T first() {
        assert(count);
        return data[0];
    }

    inline T last(u64 i = 0) {
        assert(i < count);
        return data[count - 1 - i];
    }

    inline Array<T> view(u64 start, u64 end) {
        return { data + start, end - start };
    }

    inline Array<T> advance(u64 pos) {
        return { data + pos, count - pos };
    }

    inline void swap(u64 a, u64 b) {
        auto t  = data[a];
        data[a] = data[b];
        data[b] = t;
    }

    T pop() {
        if (!count) return {};
        auto out = data[count - 1];
        count--;
        return out;
    }

    Tuple2<u64, bool> find(T item) {
        for (u64 i = 0; i < count; i++) {
            if (data[i] == item) return { i, true };
        }
        return { 0, false };
    }

    template<bool ordered = false>
    bool remove(u64 index) {
        
        if (!count) return false;
        
        if constexpr(ordered) {
            for (u64 i = index; i < count - 1; i++) {
                data[i] = data[i + 1];
            }
        } else {
            data[index] = data[count - 1];
        }
        
        count--;
        
        return true;
    }

    template<bool ordered = false, typename P>
    void remove_matches(P p) {

        for (u64 i = 0; i < count; i++) {

            while (i < count) {

                if (p(data[i])) {
                    remove<ordered>(i); // todo: n^2 in ordered case
                } else {
                    break;
                }
            }
        }
    }


    /* ---- Allocating ---- */

    static Tuple2<Array<T>, bool> alloc(u64 count, Allocator allocator = Allocators::default_heap) {
        auto data = (T*) allocator.alloc_and_clear(count * sizeof(T));
        if (!data) return {};
        return { { data, count }, true };
    }

    Tuple2<Array<T>, bool> copy(Allocator allocator = Allocators::default_heap) {
        auto [out, ok] = alloc(count, allocator);
        if (!ok) return {};
        memcpy(out.data, data, count * sizeof(T));
        return { out, ok };
    }


    /* ---- Iterators ---- */

    /*
        Note:
        I'm still not sure that I like this, but this is much readable, understandable, sane,
        and (ironically) much more powerful, than writing weird data holders,
        hardcoded begin(), end(), and a bunch of crazy operator overloads.

        If you want to write the raw iteration yourself for any reason, the code here is
        actually the example code of how you do it.

        Downsides:
        - You can't use break/continue, although you can achieve the same thing with early return, return values, etc.
        - This may have some performance cost due to the lambda.

        Same thing for other data structures.
    */
   
    template<bool with_index = false, bool reverse = false, typename P>
    inline void for_each(P p) {

        if constexpr (!reverse) {

            for (u64 i = 0; i < count; i++) {
                if constexpr (with_index) p(data[i], i);
                else                      p(data[i]);
            }

        } else {

            for (u64 i = count; i > 0;) {
                i--;
                if constexpr (with_index) p(data[i], i);
                else                      p(data[i]);
            }
        }
    }
};

template<typename T>
struct DynamicArray : Array<T> {

    u64       allocated;
    Allocator allocator;

    bool init(u64 init_count = 32, Allocator init_alloc = Allocators::default_heap) {

        assert(init_count != 0);

        *this = {};

        allocator = init_alloc;

        auto p = (T*) allocator.alloc(init_count * sizeof(T));
        if (!p) return false;

        this->data  = p;
        this->count = 0;
        allocated   = init_count;

        return true;
    }

    bool init_from(Array<T> in, Allocator init_alloc = Allocators::default_heap) {

        if (!in.count) return init(32, init_alloc);

        auto ok = init(round_to_next_power_of_2(in.count), init_alloc); // because the reason you make it dynamic is to add things
        if (!ok) return false;

        memcpy(this->data, in.data, in.count * sizeof(T));
        this->count = in.count;

        return true;
    }

    void deinit() {
        allocator.free(this->data);
        *this = {};
    }

    void reset() {
        this->count = 0;
    }

    bool reserve(u64 reserve_count) {

        if (reserve_count <= allocated) return true;

        auto p = (T*) allocator.resize(this->data, reserve_count * sizeof(T));
        if (!p) return false;

        this->data = p;
        allocated  = reserve_count;

        return true;
    }

    T* add(T item) {

        u64 old_count = this->count;
        u64 wanted    = old_count + 1;

        if (wanted > allocated) {
            auto ok = reserve(allocated * 2);
            if (!ok) return NULL;
        }

        this->data[old_count] = item;
        this->count           = wanted;

        return &this->data[old_count];
    }
};

