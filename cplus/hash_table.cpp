/* ==== Hash Table ==== */

template<typename K, typename V, u32 hash_function(K) = HashFunctions::fnv1a, bool compare(K, K) = Operators::equal_to, u64 load_factor = 70>
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
        
        assert(init_size != 0);

        *this = {};

        allocator = init_alloc;
        init_size = round_to_next_power_of_2(init_size);
        
        auto p = (Entry*) allocator.alloc_and_clear(init_size * sizeof(Entry));
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
    
    bool expand() {

        u64 new_size = size * 2;
        if (new_size < size) return false; // handle overflow

        auto new_entries = (Entry*) allocator.alloc_and_clear(new_size * sizeof(Entry)); 
        if (!new_entries) return false;

        for (u64 i = 0; i < size; i++) {

            auto it = &entries[i];
            if (!it->occupied) continue;

            auto [index, found, ok] = get_index_and_info(new_entries, new_size, it->key, it->hash);

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
            auto ok = expand();
            if (!ok) return NULL;
        }
        
        u32 hash = hash_function(key);
        auto [index, found, ok] = get_index_and_info(entries, size, key, hash);
        
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
    
    Entry* get_entry(K key) {

        u32 hash = hash_function(key);
        auto [index, found, ok] = get_index_and_info(entries, size, key, hash);

        if (!found || !ok) return NULL;
       
        return &entries[index];
    }

    Tuple2<V, bool> get_value(K key) {
        auto entry = get_entry(key);
        if (!entry) return {};
        return { entry->value, true };
    }




    /* ---- Iterators ---- */
 
    template<typename P>
    inline void for_entry(P p) {
        for (u64 i = 0; i < size; i++) {
            auto entry = &entries[i];
            if (entry->occupied) p(entry);
        }
    }

    template<typename P>
    inline void for_entry_with_index(P p) {
        for (u64 i = 0; i < size; i++) {
            auto entry = &entries[i];
            if (entry->occupied) p(entry, i);
        }
    }
   
    template<typename P>
    inline void for_key_value(P p) {
        for (u64 i = 0; i < size; i++) {
            auto entry = &entries[i];
            if (entry->occupied) p(entry->key, entry->value);
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
    
    static Tuple3<u64, bool, bool> get_index_and_info(Entry* entries, u64 size, K key, u32 hash) {
        
        u64 index = hash & (size - 1);

        u64 probe_count = 1;
        while (entries[index].occupied) {

            auto entry = &entries[index];
            if (hash == entry->hash && compare(key, entry->key))  return { index, true, true };

            index = (index + probe_count) & (size - 1);
            probe_count++;

            if (probe_count >= size) return {};
        }

        return { index, false, true };
    }
};

