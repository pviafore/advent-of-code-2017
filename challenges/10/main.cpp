#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>

#include "algo.h"
#include "containers.h"
#include "input.h"


auto createCircularBuffer() {
    containers::CircularBuffer<unsigned int, 256> cb;
    
    for(auto i = 0u; i < 256; ++i) {
        cb.append(i);
    }
    return cb;
}


auto createHashedCircularBuffer(const auto & lineLengths, unsigned int rounds=1u){
    auto cb = createCircularBuffer();
    
    auto skipSize = 0u;
    auto start = 0u;
        for(auto i = 0u; i < rounds; ++i){
        for(auto lineLength: lineLengths) {
            auto range = cb.range(start, start+lineLength);
            std::reverse(range.begin(), range.end());
            for(auto i = 0; i < lineLength; ++i) {
                cb[start+i] = range[i];
            }
            start += lineLength + skipSize++;

        }
    }
    return cb;
}

auto calculateDenseHash(const auto & newLineLengths) {
    auto values = createHashedCircularBuffer(newLineLengths, 64u).range(0,256);
    std::vector<unsigned int> out;
    for(auto i = 0; i < 16; ++i){
        auto xorValue = std::accumulate(values.begin()+i*16, values.begin()+i*16+16, 0u, std::bit_xor<unsigned int>());
        out.push_back(xorValue);
    }
    return out;
}

int main() {
    auto input = input::readSingleLineFile("input/input10.txt");
    auto lineLengths = algo::map(input::split(input, ','), input::toNumber);
    auto cb = createHashedCircularBuffer(lineLengths);
    std::cout << cb[0] * cb[1] << "\n";

    auto newLineLengths = algo::map(input, [](char c){ return static_cast<int>(c);});
    newLineLengths.push_back(17);
    newLineLengths.push_back(31);
    newLineLengths.push_back(73);
    newLineLengths.push_back(47);
    newLineLengths.push_back(23);

    auto denseHash = calculateDenseHash(newLineLengths);

    std::cout << std::hex;
    std::copy(denseHash.begin(), denseHash.end(), std::ostream_iterator<int>(std::cout));
    std::cout << "\n";

    return 0;
}
