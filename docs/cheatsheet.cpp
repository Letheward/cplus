#include "../cplus/cplus.cpp"



int main() {
   
   
    printf("\n\n==== Array & DynamicArray ====\n\n");
    {
        auto print_array = [](Array<s32> a) {
            printf("[");
            if (a.count) {
                for (u64 i = 0; i < a.count - 1; i++) {
                    printf("%d, ", a[i]);
                }
                printf("%d", a[a.count - 1]);
            }
            printf("]\n");
        };
       
        {
            s32 _a[] = {1, 2, 3, 4, 5};
            auto a = array(s32, _a);
            
            print_array(a);
            print_array(a.view(1, 3));
            print_array(a.advance(2));

            a.swap(1, 4);
            print_array(a);

            auto [index, found] = a.find(3);
            if (found) a.remove(index);
            print_array(a);
                
            insertion_sort(a, [] (auto x, auto y) { return x > y; });
            print_array(a);
            
            quick_sort(a, [] (auto x, auto y) { return x < y; });
            print_array(a);

            printf("%d\n", a.pop());
            printf("%d\n", a.pop());
            print_array(a);

            DynamicArray<s32> k;
            k.init_from_array(a);
            defer { k.deinit(); };

            print_array(k);
            
            printf("\n");
        }
        
        {
            DynamicArray<s32> a;
            a.init();
            defer { a.deinit(); };
           
            for (s32 i = 0; i < 10; i++)  a.add(i);
            print_array(a);
            
            a.reset();
            a.reserve(64);
            a.add(3);
            a.add(42);
            print_array(a);
            
            printf("a: [%llu / %llu]\n", a.count, a.allocated);
        }
    }

    
    printf("\n\n==== String & StringBuilder ====\n\n");
    {
        {
            auto s = string("Hello");
            print(string("\"@\"\n"), s);
        
            StringBuilder builder;
            builder.init_from_string(s);
            defer { builder.deinit(); };
            
            builder.append(string(" world!"));
            print(string("\"@\"\n"), builder);

            printf("StringBuilder: [%llu / %llu]\n\n", builder.count, builder.allocated);
        }
       
        {
            StringBuilder builder;
            builder.init();
            defer { builder.deinit(); };

            for (auto walk = string("foo bar baz biz"); walk.count;) {
                auto word = walk.eat_by_spaces();
                if (word.starts_with('b')) {
                    builder.print(string("[@]: "), word);
                    builder.print("%llu\n", word.count);
                }
            }

            print_string(builder);

            auto [replaced, _] = builder.replace(string(": "), string(" -> "));
            defer { free(replaced.data); };

            for (auto walk = replaced; walk.count;) {
                
                auto line   = walk.eat_line();
                auto first  = line.eat_by_spaces().trim_any_u8(string("[]"));
                auto arrow  = line.eat_by_spaces();
                auto number = line.eat_by_spaces();
                
                print(string("@ @ @\n"), first, arrow, number);
            }
        }
    }


    printf("\n\n==== HashTable ====\n\n");
    {
        HashTable<String, String> table;
        table.init(4);
        defer { table.deinit(); };

        table.add(string("red"),   string("#ff0000"));
        table.add(string("green"), string("#00ff00"));
        table.add(string("blue"),  string("#0000ff"));
        table.add(string("black"), string("#000000"));
        table.add(string("white"), string("#ffffff"));
    
        {
            auto key = string("cyan");
            auto [value, ok] = table.get_value(key);
            if (!ok) print(string("Cannot find key \"@\".\n"), key);
        }
        
        {
            auto entry = table.get_entry(string("black"));
            if (entry) print(string("\"@\" -> \"@\"\n"), entry->key, entry->value);
        }
       
        {
            auto key = string("blue");
            auto [index, ok] = table.get_index(key);
            if (ok) {
                print(string("Found key \"@\" at index "), key);
                printf("%llu\n", index);
            }
        }
        
        printf("\n");
        
        auto print_table = [&]() {
            
            printf("Table [%llu / %llu]\n", table.entry_count, table.size);
            
            table.for_entry_with_index([](auto entry, auto index) {
                printf("[%.4llu] [%.8x] | ", index, entry->hash);
                print(string("\"@\" -> \"@\"\n"), entry->key, entry->value);
            });
            
            printf("\n");
        };

        print_table();

        table.reset();
        table.add(string("hey"));

        print_table();
    }


    printf("\n\n==== Queue ====\n\n");
    {
        s32 data[8] = {0};
        auto queue = queue(s32, data);
        
        for (u64 i = 0; i < 10; i++) {
            auto ok = queue.add(rand());
            if (!ok) printf("Queue is full.\n");
        }
    
        auto [value, ok] = queue.get();
        if (ok) printf("Got: %d\n", value);
    
        auto [a, _] = queue.to_array();
        defer { free(a.data); };
    
        for (u64 i = 0; i < a.count; i++) {
            printf("%d\n", a[i]);
        }
    }


    printf("\n\n==== List ====\n\n");
    {
        List<String> list;
        list.init();

        list.add(string("foo"));
        list.add(string("bar"));
        list.add(string("baz"));
        list.add(string("biz"));

        auto print_list = [&]() {
            list.for_value([](auto value) {
                print(string("\"@\" -> "), value); 
            });
            printf("null\n");
        };

        auto print_list_reverse = [&]() {
            list.for_value<true>([](auto value) {
                print(string("\"@\" -> "), value); 
            });
            printf("null\n");
        };

        print_list();
        print_list_reverse();
        
        auto node = list.find(string("baz"));
        if (node) {
            list.remove(node);
        }

        print_list();
        print_list_reverse();
        
        list.pop();
        list.pop_from_start();
        print_list();

        list.reset();

        list.add(string("world"));
        list.add_before_start(string("hello"));

        print_list();
    }


    printf("\n\n==== MemoryArena & Allocators ====\n\n");
    {
        MemoryArena main_arena;
        main_arena.init(1024 * 32);
        defer { main_arena.deinit(); };

        auto arena = main_arena.eat_from_top(1024 * 8);
        Allocator allocator = { arena_allocator_procedure, &arena };

        for (u64 i = 0; i < 4; i++) {
            
            StringBuilder builder;
            builder.init(32, allocator);

            for (u64 j = 0; j < 128; j++) {
                builder.print("Got number: %d\n", rand());
            }

            printf("%llu\n", builder.count);
            
            allocator.reset();
        }

        arena.print_info();
    
        DynamicArray<String> a;
        a.init(32, allocator);

        for (auto s = string("foo bar baz"); s.count;) {
            auto word = s.eat_by_spaces();
            a.add(word);
        }
    
        auto [joined, _] = String::join(a, string(", "), false, allocator);
        printf("%s\n", joined.to_c_string(allocator));
        
        arena.print_info();
    }
}

