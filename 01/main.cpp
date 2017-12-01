#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>

#include "algo.h"


auto getInput() { 
    std::ifstream inFile;
    inFile.open("input/input01.txt");

    if(inFile.eof()) {
        throw std::runtime_error("The file should not have been empty)");
    }

    std::string input;
    getline(inFile, input);
    return input;

}

auto getSum(std::string input, size_t distance=1) {
    auto next = input;
    std::rotate(next.rbegin(), next.rbegin() + distance,  next.rend());

    auto zipped = zip(input, next);
    return std::accumulate(zipped.begin(), zipped.end(), 0u, [](auto sum, auto zipPair) { return sum + ( zipPair.first == zipPair.second ? zipPair.first - '0' : 0); });
}


int main() {

    auto input = getInput();;
    std::cout << getSum(input) << "\n";
    std::cout << getSum(input, input.size()/2) << "\n";
    return 0;
}