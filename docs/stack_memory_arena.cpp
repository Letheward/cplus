#include "../cplus/cplus.cpp"
#include <time.h>


int main() {
    
    u8 main_arena_data[1024 * 64] = {};
    
    auto arena       = memory_arena(main_arena_data);
    auto table_arena = arena.eat_from_top(1024 * 32);
    
    Allocator string_allocator = { arena_allocator_procedure, &arena };
    Allocator table_allocator  = { arena_allocator_procedure, &table_arena };

    srand((u32) time(0));

    auto get_random_string = [&]() -> String {
        u64 count = 5 + rand() % 8;
        auto [out, _] = String::alloc(count, string_allocator);
        for (u64 i = 0; i < out.count; i++) {
            out.data[i] = (u8) (rand() % ('z' - 'a') + 'a');
        }
        return out;
    };
    
    HashTable<String, String> table;
    table.init(16, table_allocator);

    for (u64 i = 0; i < 20; i++) {
        table.add(get_random_string(), get_random_string());
    }

    StringBuilder builder;
    builder.init(1024, string_allocator);

    table.for_all([&](auto entry, auto index) {
        builder.print("%.4llu [%.8x] ", index, entry->hash);
        if (entry->occupied) {
            builder.print(string("\"@\" -> \"@\"\n"), entry->key, entry->value);
        } else {
            builder.print("\n");
        }
    });

    print_string(builder);

    arena.print_info();
    table_arena.print_info();
}

