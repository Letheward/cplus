struct Allocator {

    enum Mode : u8 {
        ALLOC,
        RESIZE,
        FREE,
        RESET,
    };
    
    using Procedure = void* (void* data, void* old, u64 size, Mode mode);

    Procedure* proc;
    void*      data;

    inline void* alloc(u64 size) {
        return proc(data, NULL, size, Mode::ALLOC);
    }

    inline void* resize(void* old, u64 size) {
        return proc(data, old, size, Mode::RESIZE);
    }

    inline void free(void* old) {
        proc(data, old, 0, Mode::FREE);
    }
    
    inline void reset() {
        proc(data, NULL, 0, Mode::RESET);
    }
};




/* ==== Memory Arena ==== */

#define memory_arena(data)   MemoryArena { data, count_of(data), 0, 0 }    

struct MemoryArena {
    
    u8* data;
    u64 size;
    u64 allocated;
    u64 highest;

    bool init(u64 init_size) {
    
        *this = {};

        auto p = (u8*) calloc(init_size, sizeof(u8));
        if (!p) return false;
    
        data = p;
        size = init_size;

        return true;
    }

    void deinit() {
        free(data);
        *this = {};
    }
    
    MemoryArena eat_from_top(u64 new_size) {
        assert(new_size + allocated < size);
        size -= new_size;
        return { data + size, new_size, 0, 0 };
    }

    MemoryArena eat_from_bottom(u64 new_size) {
        
        assert(allocated == 0 && new_size < size);
        
        u8* old = data;
        size -= new_size;
        data += new_size;
       
        return { old, new_size, 0, 0 };
    }

    void print_info() {
        printf("MemoryArena [%llu | %llu / %llu] 0x%p\n", allocated, highest, size, data);
    }
};

void* arena_alloc(MemoryArena* a, u64 count) {

    u64 current = a->allocated;
    u64 wanted  = current + count;
    
    assert(wanted < a->size);
    
    if (wanted > a->highest) a->highest = wanted;
    a->allocated = wanted;

    return a->data + current;
}

// note: unsafe (can only resize a pointer when no other objects are allocated after that), use with caution
void* arena_resize(MemoryArena* a, void* old, u64 size) {
    
    u64 pos      = (u64) old - (u64) a->data;
    u64 wanted   = pos + size;
    
    assert(wanted < a->size);
    
    if (wanted > a->highest) a->highest = wanted; // check the highest here, maybe slow?
    a->allocated = wanted;

    return old;
}

// note: unsafe
void arena_free_size(MemoryArena* a, u64 size) {
    a->allocated -= size;
}

void arena_reset(MemoryArena* a) {
    a->allocated = 0;
    memset(a->data, 0, a->highest); // do we need this?
}





/* ==== Allocators ==== */

void* default_heap_allocator_procedure(void* data, void* old, u64 size, Allocator::Mode mode) {
    
    using Mode = Allocator::Mode;
    
    switch (mode) {
        case Mode::ALLOC:  return calloc(size, sizeof(u8)); break;
        case Mode::RESIZE: return realloc(old, size);       break;
        case Mode::FREE:   free(old);                       break;
        case Mode::RESET:                                   break;
    }
    
    return NULL;
}

void* arena_allocator_procedure(void* data, void* old, u64 size, Allocator::Mode mode) {
    
    auto p = (MemoryArena*) data;
    using Mode = Allocator::Mode;
    
    switch (mode) {
        case Mode::ALLOC:  return arena_alloc(p, size);       break;
        case Mode::RESIZE: return arena_resize(p, old, size); break; // note: unsafe
        case Mode::FREE:   break;
        case Mode::RESET:  arena_reset(p);
    }
    
    return NULL;
}

namespace Allocators {

    constexpr Allocator default_heap = { default_heap_allocator_procedure, NULL };


};


