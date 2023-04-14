/* ==== Basic Types ==== */

using u8  = unsigned char;
using u16 = unsigned short int;
using u32 = unsigned int;
using u64 = unsigned long long int;
using s8  = signed char;
using s16 = signed short;
using s32 = signed int;
using s64 = signed long long int;
using f32 = float;
using f64 = double;

static_assert(sizeof(u8)    == 1);
static_assert(sizeof(u16)   == 2);
static_assert(sizeof(u32)   == 4);
static_assert(sizeof(u64)   == 8);
static_assert(sizeof(s8)    == 1);
static_assert(sizeof(s16)   == 2);
static_assert(sizeof(s32)   == 4);
static_assert(sizeof(s64)   == 8);
static_assert(sizeof(f32)   == 4);
static_assert(sizeof(f64)   == 8);
static_assert(sizeof(void*) == 8);

