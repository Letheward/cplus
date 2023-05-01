#include "../cplus/cplus.cpp"

struct Vector3 { f32 x, y, z; };

Vector3 operator - (Vector3 a) {
    return { -a.x, -a.y, -a.z };
}

Vector3 operator + (Vector3 a, Vector3 b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector3 operator - (Vector3 a, Vector3 b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

Vector3 operator * (Vector3 a, f32 s) {
    return { a.x * s, a.y * s, a.z * s };
}

bool operator < (Vector3 a, Vector3 b) {
    return a.x < b.x && a.y < b.y && a.z < b.z;
}

void print(Vector3 v) {
    printf("[%.1f, %.1f, %.1f]", v.x, v.y, v.z);
}


int main() {

    using namespace Math;

    {
        s32 a = -7;
        s32 b = 3;
        f32 c = -3;
        f32 d = 42;
        
        printf("min(-7, 3)        = %d\n", min(a, b)); 
        printf("max(-7, 3)        = %d\n", max(a, b));
        printf("abs(-7)           = %d\n", abs(a));
        printf("clamp(-12, -7, 3) = %d\n", clamp(-12, a, b));
        
        printf("\n");

        printf("min(-3.0, 42.0)          = %.1f\n", min(c, d)); 
        printf("max(-3.0, 42.0)          = %.1f\n", max(c, d));
        printf("abs(-3.0)                = %.1f\n", abs(c));
        printf("clamp(-32.0, -3.0, 42.0) = %.1f\n", clamp((f32) -32, c, d));

        printf("\n");
    }

    {
        u32 a = 12;
        u32 b = 28;
        u32 k = gcd(a, b);
        u32 f = lcm(a, b);
        printf("gcd(12, 28) = %u\n", k);
        printf("lcm(12, 28) = %u\n", f);
    
        printf("\n");
    }
 
    {
        s32 a = -3;
        s32 b = 12;
    
        printf("-3 %% 12 = %d (bad)\n", a % b);
        printf("-3 %% 12 = %d\n", mod(a, b));

        u32 c = 12;
        u32 d = 7;

        printf("12 / 7 = %d (rounded)\n", rounded_div(c, d));
        printf("\n");
    }
    
    {
        Vector3 a = {1, 2, 3};
        Vector3 b = {1, 4, 9};
        Vector3 c = {-1, -4, -9};
    
        f32 t = (f32) 0.2;
        auto v = lerp(a, b, t);
        auto k = abs(c);

        printf("lerp(");
        print(a);
        printf(", ");
        print(b);
        printf(", %.1f) = ", t);
        print(v);
        printf("\n");
        
        printf("abs(");
        print(c);
        printf(") = ");
        print(k);
        printf("\n");
    }
}

