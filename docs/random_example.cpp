#include "../cplus/cplus.cpp"
#include <time.h>


int main() {

    u8 buffer[1024 * 64] = {};
    auto arena = memory_arena(buffer);
    
    u64 random_state = (u64) time(0);
    
    Allocator             temp   = { arena_allocator_procedure,   &arena };
    RandomNumberGenerator random = { RandomNumberGenerators::lcg, &random_state };
    
    auto random_word = [&](u64 count) -> String {
        
        auto random_character = [&]() -> u8 {
            return (u8) (random.get_u64() % ('z' - 'a') + 'a');
        };
        
        auto [out, _] = String::alloc(count, temp);
        
        u8 c = random_character();
        
        for (u64 i = 0; i < count; i++) {
            
            u8 next = 0;
            if (string("aeiouy").contains(c)) {
                next = random_character();
            } else {
                constexpr char table[] = "aaaaeeeeeiiooouuy";
                next = (u8) table[random.get_u64() % (sizeof(table) - 1)];
            }

            out.data[i] = c;
            c = next;
        }

        return out;
    };
    
    StringBuilder builder;
    builder.init();
    defer { builder.deinit(); };
    
    for (u64 i = 0; i < 1024; i++) {
        
        builder.append(random_word(random.get_u64() % 10 + 2));
        
        u8 p = random.get_u64() % 16;
        switch (p) {
            
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                builder.append(' ');
                break;
            case 12:
            case 13:
                builder.append(string(", "));
                break;
            case 14:
                builder.append(string(". "));
                break;
            case 15:
                builder.append('\n');
                break;
        }
    }
   
    print_string(builder);
    printf("\n");
}

