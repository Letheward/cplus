// Before the #include, define a non-empty CPLUS_NAMESPACE
#define CPLUS_NAMESPACE CP
#include "../cplus/cplus.cpp"

int main() {

    // Now you need to use CP:: for most things, 
    // including some macros (they are not really namespaced, they just look like that)
    for (auto k = CP::string("this is some words"); k.count;) {
        auto word = k.eat_by_spaces();
        defer { printf(": %llu\n", word.count); }; // defer is not namespaced  
        CP::print(CP::string("[@]"), word);
    }
}

