#include "../cplus/cplus.cpp"


int main() {

    printf("\n\n==== Print ====\n\n");
    {
        printf("print c strings\n");
        
        auto hello = string("Hello");
        print_string(hello);
        printf("\n");

        print(string("@, world!\n"), hello);
    }

    printf("\n\n==== Read Input ====\n\n");
    {
        u8 buffer[1024] = {};

        while (1) {
            
            printf("> ");
            auto line = read_line(array_string(buffer));
            if (line == string("quit")) break;
            
            for (auto s = line; s.count;) {
                print(string("[@]\n"), s.eat_by_spaces());
            }
        }
    }

    printf("\n\n==== File IO ====\n\n");
    {
        auto [file, _] = load_file(__FILE__);
        defer { free(file.data); };

        StringBuilder builder;
        builder.init();
        defer { builder.deinit(); };

        for (auto s = file; s.count;) {
            builder.print(string("@ "), s.eat_by_any_separators(string(" \t\r\n,.;:+-*/=\\<>()[]{}!#%^&'\"")));
        }

        builder.append('\n');

        print_string(builder);
        //save_file(builder, "words.txt");
    }
}

