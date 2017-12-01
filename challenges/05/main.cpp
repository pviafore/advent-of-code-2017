#include <iostream>
#include <string>

#include "algo.h"
#include "input.h"

auto getStepsPart1(auto numbers) {
    auto index = 0u;
    auto steps = 0u;
    while(index < numbers.size()) {
        index += numbers[index]++;
        steps++;
    }
    return steps;
}

auto getStepsPart2(auto numbers) {
    auto index = 0u;
    auto steps = 0u;
    while(index < numbers.size()) {
        auto modifier = (numbers[index] >= 3)  ? -1 : 1;
        auto old_index = index;
        index += numbers[old_index];
        numbers[old_index] += modifier;
        steps++;
    }
    return steps;
}

int main() {
    auto numbers = algo::map(input::readMultiLineFile("input/input05.txt"), input::toNumber);
    std::cout << getStepsPart1(numbers) << "\n";
    std::cout << getStepsPart2(numbers) << "\n";
    return 0;
}