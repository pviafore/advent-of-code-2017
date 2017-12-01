#include <iostream>

constexpr unsigned int GENERATOR_A_FACTOR = 16807;
constexpr unsigned int GENERATOR_B_FACTOR = 48271;

unsigned int get_matching_pairs(unsigned int factorA, unsigned int factorB, unsigned int times,
                                unsigned int maskA = 0x0, unsigned int maskB = 0x0) {
    unsigned long generatorA = 516;
    unsigned long generatorB = 190;
    unsigned int count = 0u;
    for(auto _ = 0u; _ < times; ++_){
        do{
            generatorA = generatorA * factorA % 2147483647;
        } while((generatorA & maskA) != 0);
        do {
            generatorB = generatorB * factorB % 2147483647;
        } while((generatorB & maskB) != 0);
        if ((generatorA & 0xFFFF) == (generatorB & 0xFFFF)){
            ++count;
        }
    }
    return count;
}

int main(){
    std::cout << "Matching pairs in first 40 million: "
              << get_matching_pairs(GENERATOR_A_FACTOR, GENERATOR_B_FACTOR, 40'000'000)
              << "\n";
    std::cout << "Matching pairs in first 5 million (more aligned): "
              << get_matching_pairs(GENERATOR_A_FACTOR, GENERATOR_B_FACTOR, 5'000'000,
                                    0x3, 0x7) // mask 2 bits or three bits respectively
              << "\n";
    return 0;
}
