# C+

A tiny standard library for C++.

## Overview

An overview of files:

~~~
cplus.cpp         includes all the things 

core.cpp          basic type definitions  
utils.cpp         defer, tuple, bit utils, compile time type utils
math.cpp          basic math utils
allocator.cpp     allocator interface and a simple arena allocator

array.cpp         array view and dynamic array
queue.cpp         fixed size queue
list.cpp          doubly linked list with a fixed size pool
sort.cpp          basic sorting algorithms

string.cpp        string view, string builder, inplace string
parse.cpp         basic string parsing utils
unicode.cpp       unicode decode/encode/convert
io.cpp            console IO and load/save file

hash.cpp          basic hash functions
hash_table.cpp    simple hash table
~~~

## Simple Example

Create a `hello.cpp` file in an empty folder, and put the folder `cplus` there:

~~~ c++
// hello.cpp
#include "cplus/cplus.cpp"

int main() {

    auto hello = string("Hello, world! From C+, the tiny standard library.");

    for (auto remains = hello; remains.count;) {
        auto word = remains.eat_by_any_u8_separators(string(" ,.!"));
        print(string("[@]\n"), word);
    }
}
~~~

Compile and run it:

~~~ sh
g++ hello.cpp -o hello && ./hello  
~~~

It should output something like this:
~~~
[Hello]
[world]
[From]
[C+]
[the]
[tiny]
[standard]
[library]
~~~

## Why

It's not meant to be faster or safer or more powerful, or even more correct than STL.

But:
- It is drastically smaller and simpler.
- It has much better interfaces.
- It compiles much faster.
- It has (relatively) better error messages when things go wrong.
- It's allocation/allocator aware. 

I don't recommend using this in production, but if you want to do some prototyping or just program for fun, and also don't like STL, this library might be a good fit. 

## Notes

- Currently, C+ does not support multiple compilation units, so you can only do unity build.
- I don't use exceptions at all. If you also don't, you can use the flag `-fno-exceptions` or similiar compiler switches.

## Dependencies

- A C++17 compiler: C+ is written in C++17.
- libc: C+ uses some C standard library functions.

## License

This repository is dedicated to public domain.

