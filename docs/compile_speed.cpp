#if 0

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

    std::vector<uint32_t> v;
    std::queue<uint32_t> q;
    std::list<uint32_t> l;
    std::string s;
    std::unordered_map<std::string_view, std::string_view> m;

    std::cout << "Hello, World!" << std::endl;
}

#else

#include "../cplus/cplus.cpp"

int main() {

    // we want a fair comparison, so we need to do "constructors" and "destructors" manually 
    
    DynamicArray<u32> a;
    Queue<u32> q;
    List<u32> l;
    StringBuilder b;
    HashTable<String, String> t;
    
    a.init();
    q.init();
    l.init();
    b.init();
    t.init();
    
    a.deinit();
    q.deinit();
    l.deinit();
    b.deinit();
    t.deinit();

    printf("Hello, World!\n");
}

#endif

