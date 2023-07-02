struct Random {
    
    using Procedure = u64 (void*);
    
    Procedure* proc;
    void*      data;

    inline u64 get_u64() {
        return proc(data);
    }

    // in [0, 1]
    inline f32 get_f32() {
        constexpr u64 range = ((u64) 1 << 23) - 1;
        u64 x = proc(data);
        return (f32) (x & range) / (f32) range;
    }

    // in [0, 1]
    // note: the u64 output must be greater or equal to range, for example, lcg() can't do this
    inline f64 get_f64() {
        constexpr u64 range = ((u64) 1 << 53) - 1;
        u64 x = proc(data);
        return (f64) (x & range) / (f64) range;
    }
};

namespace RandomNumberGenerators {

    u64 lcg(void* p) {

        constexpr u64 a = 6364136223846793005;
        constexpr u64 c = 1442695040888963407;

        auto state = (u64*) p;
        u64 x = *state;
        
        x = x * a + c; // overflow so m = 2^64
        
        *state = x;

        return x >> 32; // lower bits have very poor quality 
    }
    
    // note: must use a non-zero seed
    u64 xorshift_star(void* p) {

        constexpr u64 c = 0x2545f4914f6cdd1d;
        
        auto state = (u64*) p;
        u64 x = *state;
        
        x ^= x >> 12;
        x ^= x << 25;
        x ^= x >> 27;
        
        *state = x;

        return x * c;
    }
}

