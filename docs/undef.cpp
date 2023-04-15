// if you don't like macro leaks, you can define this, and define you own macros later
// this works with CPLUS_NAMESPACE

#define CPLUS_UNDEF_MACROS 
#include "../cplus/cplus.cpp"

#define my_string(s)   String {(u8*) s, sizeof(s) - 1}

int main() {
    
    for (auto k = my_string("this is some words"); k.count;) {
        auto word = k.eat_by_spaces();
        print(my_string("[@]\n"), word);
    }
}

