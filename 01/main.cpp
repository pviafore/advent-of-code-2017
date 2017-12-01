#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


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

    auto nextIter = next.begin();
    auto sum = 0u;
    for(auto current : input) {
        if (current == *nextIter) {
            sum += current - '0';
        }
        ++nextIter;
    }

    return sum;

}


int main() {

    auto input = getInput();;
    std::cout << getSum(input) << "\n";
    std::cout << getSum(input, input.size()/2) << "\n";
    return 0;
}