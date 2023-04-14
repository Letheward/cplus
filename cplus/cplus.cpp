#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>   
#include <assert.h>


#ifdef CPLUS_NAMESPACE
namespace CPLUS_NAMESPACE {
#endif


#include "core.cpp"
#include "utils.cpp"
#include "math.cpp"
#include "allocator.cpp"

#include "array.cpp"
#include "queue.cpp"
#include "list.cpp"
#include "sort.cpp"

#include "string.cpp"
#include "parse.cpp"
#include "unicode.cpp"
#include "io.cpp"

#include "hash.cpp"
#include "hash_table.cpp"


#ifdef CPLUS_NAMESPACE
}
#endif

