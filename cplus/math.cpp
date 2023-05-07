/* ==== Constants ==== */

namespace Numbers {
    
    constexpr f32 TAU   = (f32) 6.283185307179586476925286766559;
    constexpr f64 TAU64 = (f64) 6.283185307179586476925286766559;

    constexpr f32 PI    = (f32) 3.141592653589793238462643383279;
    constexpr f64 PI64  = (f64) 3.141592653589793238462643383279;
                                
    constexpr f32 E     = (f32) 2.718281828459045235360287471352;
    constexpr f32 E64   = (f32) 2.718281828459045235360287471352;
}




/* ==== Basic Math Utils ==== */

namespace Math {
    
    template<typename T>
    T abs(T x) {
        return x < T {} ? -x : x;
    }

    template<typename T>
    T min(T a, T b) {
        return a < b ? a : b;
    }

    template<typename T>
    T max(T a, T b) {
        return a > b ? a : b;
    }

    template<typename T>
    T clamp(T x, T low, T high) {
        if (x < low ) return low;
        if (x > high) return high;
        return x;
    }

    template<typename T, typename F>
    T lerp(T a, T b, F t) {
        static_assert(type_is_float<F>());
        return a + (b - a) * t;
    }

    template<typename T>
    T mod(T x, T m) {
        return ((x % m) + m) % m;
    }

    template<typename T>
    T gcd(T a, T b) {

        if constexpr (type_is_signed_integer<T>()) {
            a = abs(a);
            b = abs(b);
        } else {
            static_assert(type_is_unsigned_integer<T>());
        }
        
        while (b) {
            auto t = b;
            b = a % b;
            a = t;
        }

        return a;
    }

    template<typename T>
    T lcm(T a, T b) {
        
        T d;

        if constexpr (type_is_signed_integer<T>()) {
            a = abs(a);
            b = abs(b);
            d = (T) gcd((u64) a, (u64) b); // avoid abs() in gcd()
        } else {
            static_assert(type_is_unsigned_integer<T>());
            d = gcd(a, b);
        }
        
        return a * (b / d); 
    }

    template<typename T>
    Tuple2<T, T> div_mod(T a, T b) {
        static_assert(type_is_unsigned_integer<T>());
        return { a / b, a % b };
    }

    template<typename T>
    T rounded_div(T a, T b) {
        auto [out, left] = div_mod(a, b);
        return out + (left * 2 > b);
    }
}




/* ==== Powers of 2 Algorithms ==== */

// From Hacker's Delight

template<typename T>
bool is_power_of_2(T x) {
    static_assert(type_is_unsigned_integer<T>());
    return (x & (x - 1)) == 0;
}

template<typename T>
bool is_power_of_2_minus_1(T x) {
    static_assert(type_is_unsigned_integer<T>());
    return (x & (x + 1)) == 0;
}

template<typename T>
T round_to_prev_power_of_2(T x) {
    
    static_assert(type_is_unsigned_integer<T>());
    
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    if constexpr (sizeof(T) > 8)  x |= (x >> 8);
    if constexpr (sizeof(T) > 16) x |= (x >> 16);
    if constexpr (sizeof(T) > 32) x |= (x >> 32);
    
    return x - (x >> 1);
}

template<typename T>
T round_to_next_power_of_2(T x) {

    static_assert(type_is_unsigned_integer<T>());
    
    x = x - 1;
    
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    if constexpr (sizeof(T) > 8)  x |= (x >> 8);
    if constexpr (sizeof(T) > 16) x |= (x >> 16);
    if constexpr (sizeof(T) > 32) x |= (x >> 32);
    
    return x + 1;
}


