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


#ifdef CPLUS_UNDEF_MACROS

#undef CPLUS_UNDEF_MACROS   
#undef CPLUS_NAMESPACE

#undef ___defer___
#undef __defer__
#undef defer 

#undef count_of    
#undef array  
#undef queue  
#undef memory_arena 

#undef string          
#undef utf8_string     
#undef utf16_string    
#undef utf32_string    
#undef array_string    
#undef data_string     
#undef quoted_string   

#endif

