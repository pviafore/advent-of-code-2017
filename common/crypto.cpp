
#include "crypto.h"

#include <algorithm>
#include <numeric>

#include "algo.h"

namespace crypto {

    std::unique_ptr<containers::CircularBuffer<unsigned int, 256>> createCircularBuffer() {
        auto cb = std::make_unique<containers::CircularBuffer<unsigned int, 256>>();
        
        for(auto i = 0u; i < 256; ++i) {
            cb->append(i);
        }
        return cb;
    }


    std::unique_ptr<containers::CircularBuffer<unsigned int, 256>> createHashedCircularBuffer(const std::vector<int> & lineLengths, unsigned int rounds){
        auto cb = createCircularBuffer();
        auto skipSize = 0u;
        auto start = 0u;
            for(auto i = 0u; i < rounds; ++i){
            for(auto lineLength: lineLengths) {
                auto range = cb->range(start, start+lineLength);
                std::reverse(range.begin(), range.end());
                for(auto i = 0; i < lineLength; ++i) {
                    (*cb)[start+i] = range[i];
                }
                start += lineLength + skipSize++;

            }
        }
        return cb;
    }

    std::vector<unsigned int> calculateDenseHash(const std::string& input) {

        auto newLineLengths = algo::map(input, [](char c){ return static_cast<int>(c);});
        newLineLengths.push_back(17);
        newLineLengths.push_back(31);
        newLineLengths.push_back(73);
        newLineLengths.push_back(47);
        newLineLengths.push_back(23);
        
        auto values = createHashedCircularBuffer(newLineLengths, 64u)->range(0,256);
        std::vector<unsigned int> out;
        for(auto i = 0; i < 16; ++i){
            auto xorValue = std::accumulate(values.begin()+i*16, values.begin()+i*16+16, 0u, std::bit_xor<unsigned int>());
            out.push_back(xorValue);
        }
        return out;
    }
}