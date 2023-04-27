#include "../cplus/cplus.cpp"


int main() {

    MemoryArena arena;
    arena.init(1024 * 1024);
    defer { arena.deinit(); };
   
    auto temp_arena = arena.eat_from_top(1024 * 512);
    Allocator temp = { arena_allocator_procedure, &temp_arena };

    HashTable<String, String> table;
    table.init(256, { arena_allocator_procedure, &arena });

    auto [buffer, _] = String::alloc(1024, temp);

    while (1) {

        printf("> ");

        auto line    = read_line(buffer);
        auto command = line.eat_by_spaces();
        
        if (command == string("help")) {

            puts( 
R"end(
How to use:
    
    help
        Print this info.
    
    quit
        Quit the program.

    set <key> <value>
        Set key value pair in the table.

    get <key>
        Get value base on key.

    list
        List all key value pairs.
)end"
            );
       
        } else if (command == string("quit")) {
            
            break;

        } else if (command == string("set")) {
            
            auto [key,   __]  = line.eat_by_spaces().copy(temp);
            auto [value, ___] = line.eat_by_spaces().copy(temp);
            table.add(key, value);
        
        } else if (command == string("get")) {

            auto key = line.eat_by_spaces();
            auto [value, ok] = table.get_value(key);
            if (!ok) print(string("There is no key \"@\"!\n"), key);
            else     print(string("\"@\"\n"), value);
        
        } else if (command == string("list")) {
            
            table.for_key_value([](auto k, auto v) {
                print(string("\"@\" -> \"@\"\n"), k, v);
            });
        
        } else {
            
            print(string("Error: Unknown command \"@\". Type \"help\" to list all commands.\n"), command);
        }
    }

    arena.print_info();
    temp_arena.print_info();
}

