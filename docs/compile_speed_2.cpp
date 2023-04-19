#include "../cplus/cplus.cpp"

struct GenericType {
    const char* name;
    u64         count;
};

void generate_code(const char* start, Array<GenericType> generic_types, Array<const char*> types, bool do_init_deinit = false) {  

    char counter[] = { 'a', 'a', 'a', 'a', 0 };
     
    auto increment = [&]() {
        
        const u64  w = count_of(counter) - 1;
        const char n = 'a' + 26;
        
        counter[0]++; 
        for (u64 i = 0; i < w; i++) {
            if (counter[i] >= n) {
                counter[i] = 'a';
                counter[i + 1]++;
            }
        }
    };
   
    printf("%s", start);
           
    for (u64 i = 0; i < generic_types.count; i++) {
        
        auto [name, count] = generic_types[i];

        if (count == 2) {
            
            for (u64 j = 0; j < types.count; j++) {
                for (u64 k = 0; k < types.count; k++) {
                    auto t1 = types[j];
                    auto t2 = types[k];
                    printf("    %s<%s, %s> %s;\n", name, t1, t2, counter);
                    if (do_init_deinit) {
                        printf("    %s.init();\n", counter);
                        printf("    defer { %s.deinit(); };\n\n", counter);
                    }
                    increment();
                }
            }

        } else {
            
            for (u64 j = 0; j < types.count; j++) {
                auto t = types[j];
                printf("    %s<%s> %s;\n", name, t, counter);
                if (do_init_deinit) {
                    printf("    %s.init();\n", counter);
                    printf("    defer { %s.deinit(); };\n\n", counter);
                }
                increment();
            }
        }
    }

    printf("}\n");
}



int main() {

    if (1) {
         
        GenericType a[] = {
            { "std::vector",        1 },
            { "std::queue",         1 }, 
            { "std::list",          1 }, 
            { "std::unordered_map", 2 }, 
        };

        const char* b[] = {
            "uint8_t",
            "uint16_t",
            "uint32_t",
            "uint64_t",
            "int8_t",
            "int16_t",
            "int32_t",
            "int64_t",
            "std::string", 
            "std::string_view", 
        };

        const char* start = R"end(
#include <tuple>
#include <functional>
#include <numeric>
#include <memory>

#include <vector>
#include <queue>
#include <list>
#include <algorithm>

#include <string_view>
#include <string>
#include <iostream>

#include <unordered_map>

int main() {

)end";

        generate_code(start, array(GenericType, a), array(const char*, b));

    } else {

        GenericType a[] = {
            { "DynamicArray", 1 },
            { "Queue",        1 }, 
            { "List",         1 }, 
            { "HashTable",    2 }, 
        };

        const char* b[] = {
            "u8",
            "u16",
            "u32",
            "u64",
            "s8",
            "s16",
            "s32",
            "s64",
            "StringBuilder", 
            "String", 
        };

        const char* start = R"end(
#include "cplus/cplus.cpp"

int main() {

)end";

        generate_code(start, array(GenericType, a), array(const char*, b), true);
    }
}

