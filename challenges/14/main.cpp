#include <iostream>
#include <string>

#include "algo.h"
#include "crypto.h"


auto getNumberOfSquaresLit(const std::string& input) {
    auto range = algo::getNumericRange(0,128);
    auto strings = algo::map(range, [input](int i) { return input + "-" + std::to_string(i);});
    auto knotHashes = algo::map(strings, crypto::calculateDenseHash );
    return knotHashes;
}

int main() {
    auto input = std::string("hfdlxzhv");
    auto value = getNumberOfSquaresLit("input");
    std::cout << value << "\n";
    return 0;
}