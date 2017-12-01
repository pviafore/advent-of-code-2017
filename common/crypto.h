#ifndef CRYPTO_H_
#define CRYPTO_H_

#include <memory>

#include "containers.h"
#include <vector>
namespace crypto {
    std::unique_ptr<containers::CircularBuffer<unsigned int, 256>> createCircularBuffer();
    std::unique_ptr<containers::CircularBuffer<unsigned int, 256>> createHashedCircularBuffer(const std::vector<int> & lineLengths, unsigned int rounds=1u);
    std::vector<unsigned int> calculateDenseHash(const std::string & input);
        
}
#endif