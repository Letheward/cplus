namespace HashFunctions {

    u32 fnv1a(String s) {

        u32 hash = 0x811c9dc5;
        for (u64 i = 0; i < s.count; i++) {
            hash ^= (u32) s[i];
            hash *= 0x01000193;
        }

        return hash;
    }

    u32 djb2(String s) {

        u32 hash = 5381;
        for (u64 i = 0; i < s.count; i++) {
            hash += (hash << 5) + s[i];
        }

        return hash;
    }

    // todo: is this broken? collision count is way too high
    u32 pjw(String s) {

        u32 hash = 0;

        for (u64 i = 0; i < s.count; i++) {

            hash = (hash << 4) + s[i];

            u32 test = hash & 0xf0000000;
            if (test)  hash = (hash ^ (test >> 24)) & ~0xf0000000;
        }

        return hash;
    }



    /* ==== Generate Code ==== */
    
    #define get(func)                \
    template<typename T>             \
    u32 func(T s) {                  \
        return func(data_string(s)); \
    }                                \

    get(fnv1a)
    get(djb2)
    get(pjw)

    #undef get
}

