#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>

#include "algo.h"
#include "input.h"

//a raw loop probably is way more readable in this case, since you loop indexes and (compare index+distance) % size
//however, I wanted to see what it would be like without raw loops in the code
//So, I'm instead rotating a copy by the distance, zipping the strings into a pair, and accumulating any pairs that match

auto getSum(const std::string &input, size_t distance=1) {
    auto next = input;
    std::rotate(next.rbegin(), next.rbegin() + distance,  next.rend());

    auto zipped = algo::zip(input, next);
    auto addValueIfMatching = [](auto sum, auto zipPair) { return sum + ( zipPair.first == zipPair.second ? zipPair.first - '0' : 0); };
    return std::accumulate(zipped.begin(), zipped.end(), 0u, addValueIfMatching);
}

int main() {
    auto input = input::readSingleLineFile("input/input01.txt");
    std::cout << getSum(input) << "\n";
    std::cout << getSum(input, input.size()/2) << "\n";
    return 0;
}