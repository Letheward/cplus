#include "../cplus/cplus.cpp"

int main() {

    u64 seed = 1;
    RandomNumberGenerator random = { RandomNumberGenerators::lcg, &seed };

    for (u64 i = 0; i < 256; i++) {
        auto k = random.get_u64();
        printf("%.16llx | %llu\n", k, k);
    }
}

