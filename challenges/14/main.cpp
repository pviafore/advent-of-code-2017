#include <iostream>
#include <string>

#include "algo.h"
#include "crypto.h"


auto getNumberOfSquaresLit(const std::string& input) {
    auto range = algo::getNumericRange(0,128);
    auto strings = algo::map(range.begin(), range.end(), [input](int i) { return input + "-" + std::to_string(i);});
    auto knotHashes = algo::map(strings.begin(), strings.end(), )
}

int main() {
    auto input = std::string("hfdlxzhv");
    return 0;
}