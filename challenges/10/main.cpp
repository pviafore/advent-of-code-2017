#include <algorithm>
#include <iostream>
#include <iterator>

#include "algo.h"
#include "crypto.h"
#include "input.h"


int main() {
    auto input = input::readSingleLineFile("input/input10.txt");
    auto lineLengths = algo::map(input::split(input, ','), input::toNumber);
    auto &cb = *crypto::createHashedCircularBuffer(lineLengths);
    std::cout << cb[0] * cb[1] << "\n";

    auto denseHash = crypto::calculateDenseHash(input);

    std::cout << std::hex;
    std::copy(denseHash.begin(), denseHash.end(), std::ostream_iterator<int>(std::cout));
    std::cout << "\n";

    return 0;
}
