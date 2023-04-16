/* ==== Hash Table ==== */

template<typename K, typename V, u32 hash_function(K) = HashFunctions::fnv1a, u64 load_factor = 70>
struct HashTable {

    struct Entry {
        K   key;
        V   value;
        u32 hash;
        u32 occupied;
    };

    Entry*    entries;
    u64       entry_count;
    u64       size;
    Allocator allocator;

    bool init(u64 init_size = 256, Allocator init_alloc = Allocators::default_heap) {
        
        *this = {};

        allocator = init_alloc;
        init_size = round_to_next_power_of_2(init_size);
        
        auto p = (Entry*) allocator.alloc(init_size * sizeof(Entry));
        if (!p) return false;

        this->entries     = p; 
        this->entry_count = 0;
        this->size        = init_size;
        
        return true;
    }

    void deinit() {
        allocator.free(entries);
        *this = {};
    }

    void reset() {
        memset(entries, 0, sizeof(Entry) * size);
        entry_count = 0;
    }
    
    bool resize() {

        u64 new_size = size * 2;
        if (new_size < size) return false; // handle overflow

        auto new_entries = (Entry*) allocator.alloc(new_size * sizeof(Entry));
        if (!new_entries) return false;

        // re-slot all old entries. todo: is this slow?
        for (u64 i = 0; i < size; i++) {

            auto it = &entries[i];
            if (!it->occupied) continue;

            auto [index, found, ok] = internal_get_index_and_stats(new_entries, new_size, it->key, it->hash);

            if (!ok) return false;
            if (found) continue;

            new_entries[index] = *it;
        }

        allocator.free(entries);
        entries = new_entries;
        size    = new_size;

        return true;
    }

    Entry* add(K key, V value = {}) {

        if ((entry_count + 1) * 100 > size * load_factor) {
            auto ok = resize();
            if (!ok) return NULL;
        }
        
        u32 hash = hash_function(key);
        auto [index, found, ok] = internal_get_index_and_stats(entries, size, key, hash);
        
        if (!ok) return NULL;

        auto entry = &entries[index];
        
        if (found) {
            entry->value = value;
            return entry;
        }

        *entry = { key, value, hash, 1 };
        entry_count++;

        return entry;
    }
    
    Tuple2<u64, bool> get_index(K key) {
        u32 hash  = hash_function(key);
        auto [index, found, ok] = internal_get_index_and_stats(entries, size, key, hash);
        if (!found || !ok) return { 0, false };
        return { index, true };
    }

    Entry* get_entry(K key) {
        auto [index, ok] = get_index(key);
        if (!ok) return NULL;
        return &entries[index];
    }

    Tuple2<V, bool> get_value(K key) {
        auto [index, ok] = get_index(key);
        if (!ok) return {};
        return { entries[index].value, true };
    }




    /* ---- Iterators ---- */

    template<typename P>
    inline void for_key_value(P p) {
        for (u64 i = 0; i < size; i++) {
            auto entry = &entries[i];
            if (!entry->occupied) continue;
            p(entry->key, entry->value);
        }
    }
 
    template<typename P>
    inline void for_entry(P p) {
        for (u64 i = 0; i < size; i++) {
            auto entry = &entries[i];
            if (!entry->occupied) continue;
            p(entry);
        }
    }

    template<typename P>
    inline void for_entry_with_index(P p) {
        for (u64 i = 0; i < size; i++) {
            auto entry = &entries[i];
            if (!entry->occupied) continue;
            p(entry, i);
        }
    }
   
    template<typename P>
    inline void for_all(P p) {
        for (u64 i = 0; i < size; i++) {
            auto entry = &entries[i];
            p(entry, i);
        }
    }

private:
    
    static Tuple3<u64, bool, bool> internal_get_index_and_stats(Entry* entries, u64 size, K key, u32 hash) {
        
        u64 index = hash & (size - 1);

        u64 probe_count = 1;
        while (entries[index].occupied) {

            auto entry = &entries[index];
            if (hash == entry->hash && key == entry->key)  return { index, true, true };

            index = (index + probe_count) & (size - 1);
            probe_count++;

            if (probe_count >= size) return {};
        }

        return { index, false, true };
    }
};

