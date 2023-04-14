/* ==== Type Utils ==== */

// todo: validate

template<typename T>
constexpr bool is_float() {
    return ((T) 0.01) != 0;
}

template<typename T>
constexpr bool is_integer() {
    return !is_float<T>();
}

template<typename T>
constexpr bool is_signed_integer() {
    return ((T) -1) < 0 && is_integer<T>();
}

template<typename T>
constexpr bool is_unsigned_integer() {
    return ((T) -1) > 0;
}




/* ==== Bit Utils ==== */

template<typename A, typename B>
inline B bit_cast(A a) {
    union { A a; B b; } out = {};
    out.a = a;
    return out.b;
}

template<typename T>
inline T get_mask(T c) {
    
    static_assert(is_integer<T>() && sizeof(T) <= 8);
    
    if constexpr (sizeof(T) == 1) return (u8)  (((s8)  ((u8)  c | -(u8)  c)) >> 7);
    if constexpr (sizeof(T) == 2) return (u16) (((s16) ((u16) c | -(u16) c)) >> 15);
    if constexpr (sizeof(T) == 4) return (u32) (((s32) ((u32) c | -(u32) c)) >> 31);
    if constexpr (sizeof(T) == 8) return (u64) (((s64) ((u64) c | -(u64) c)) >> 63);
}




/* ==== Tuples ==== */

#if 0

void generate_tuple_code(u64 count = 6) {

    for (u64 i = 2; i <= count; i++) {
        printf("template<");
        for (u64 j = 0; j < i - 1; j++) {
            printf("typename T%llu, ", j);
        }
        printf("typename T%llu>\n", i - 1);
        printf("struct Tuple%llu { ", i);
        for (u64 j = 0; j < i; j++) {
            printf("T%llu v%llu; ", j, j);
        }
        printf("};\n\n");
    }
}

#endif

template<typename T0, typename T1>
struct Tuple2 { T0 v0; T1 v1; };

template<typename T0, typename T1, typename T2>
struct Tuple3 { T0 v0; T1 v1; T2 v2; };

template<typename T0, typename T1, typename T2, typename T3>
struct Tuple4 { T0 v0; T1 v1; T2 v2; T3 v3; };

template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct Tuple5 { T0 v0; T1 v1; T2 v2; T3 v3; T4 v4; };

template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
struct Tuple6 { T0 v0; T1 v1; T2 v2; T3 v3; T4 v4; T5 v5; };




/* ==== Deferer ==== */

#define ___defer___(N) __defer__ ## N
#define __defer__(N)   ___defer___(N)

#ifdef CPLUS_NAMESPACE
#define defer auto __defer__(__LINE__) = CPLUS_NAMESPACE::Deferer {} + [&]()
#else
#define defer auto __defer__(__LINE__) = Deferer {} + [&]()
#endif

struct Deferer {

    template<typename P>
    struct Holder {
        P proc;
        Holder(P p) : proc(p) { }
        ~Holder() { proc(); }
    };

    template<typename P>
    Holder<P> operator + (P p) {
        return Holder<P>(p);
    }
};


