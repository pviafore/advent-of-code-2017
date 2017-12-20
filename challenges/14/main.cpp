#include <bitset>
#include <deque>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>


#include "algo.h"
#include "crypto.h"
#include "input.h"

using Pos = std::pair<size_t, size_t>;
const auto SIZE = 128;

std::string toBinaryString(const std::vector<unsigned int>& v) {
    return input::join(algo::map(v, [](unsigned int i) { return std::bitset<8>(i).to_string();}));
}


auto getBitstring(const std::string& input) {
    auto range = algo::getNumericRange(0,SIZE);
    auto strings = algo::map(range, [input](int i) { return input + "-" + std::to_string(i);});
    auto knotHashes = algo::map(strings, crypto::calculateDenseHash );
    auto binaryStrings = algo::map(knotHashes, toBinaryString);
    return input::join(binaryStrings);
}

auto convertIndex(size_t index) { 
    return Pos(index % SIZE, index / SIZE);
}

auto convertPosition(Pos pos) {
    return pos.second * SIZE + pos.first;
}

auto calculateAdjacentPositions(const std::string& bitstring, Pos position) {
    auto left = position, right = position, up = position, down = position;
    left.first--;
    right.first++;
    up.second--;
    down.second++;
    std::vector<Pos> positions;
    if (position.first != 0 && bitstring[convertPosition(left)] == '1') {
        positions.push_back(left);
    }
    if(right.first < SIZE && bitstring[convertPosition(right)] == '1') {
        positions.push_back(right);
    }
    if (position.second != 0 && bitstring[convertPosition(up)] == '1') {
        positions.push_back(up);
    }
    if (down.second < SIZE && bitstring[convertPosition(down)] == '1') {
        positions.push_back(down);
    }
    return positions;
}

auto getNumberOfRegions(std::string bitstring) {
    auto numberOfRegions = 0u;
    size_t index = bitstring.find('1');
    
    while( index != std::string::npos) {
        numberOfRegions++;
        std::deque<Pos> positions = {convertIndex(index)};
        while(!positions.empty()) {
            
            auto position = positions.front();
            positions.pop_front();
            bitstring[convertPosition(position)] = '0';

            auto nextPositions = calculateAdjacentPositions(bitstring, position);
            std::copy(nextPositions.begin(), nextPositions.end(), std::back_inserter(positions));
        }
        index = bitstring.find('1');
    }
    return numberOfRegions;

}

int main() {
    auto bitstring = getBitstring("hfdlxzhv");
    auto numberOfUsedSquares = std::count(bitstring.begin(), bitstring.end(), '1');
    std::cout << numberOfUsedSquares << "\n";
    
    std::cout << getNumberOfRegions(bitstring) << "\n";

    return 0;
}