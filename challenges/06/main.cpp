#include <algorithm>
#include <assert.h>
#include <iostream>
#include <set>

#include "algo.h"
#include "input.h"

auto getNumberOfTimesBeforeSeeingDuplicates(const auto &input) {
    auto numbers = input;
    std::vector<decltype(numbers)> v;
    
    auto numberOfTimesThroughLoop = 0u;
    while(std::find(v.begin(), v.end(), numbers) == v.end()) {
        v.push_back(numbers);
        auto max_element = std::max_element(numbers.begin(), numbers.end());

        assert(max_element != numbers.end());

        const auto max_value = *max_element;
        *max_element = 0;
        algo::apply_cyclically(numbers, max_element+1, max_value, [](auto& s) { s += 1;});

        numberOfTimesThroughLoop++;
    }
    auto distance = std::distance(std::find(v.begin(), v.end(), numbers), v.end());
    return std::make_pair(numberOfTimesThroughLoop, distance);
    
}

int main() {
    auto numbers = input::toNumbers(input::readSingleLineFile("input/input06.txt"));
    
    auto answer = getNumberOfTimesBeforeSeeingDuplicates(numbers);
    std::cout << answer.first << " " << answer.second << "\n";
    
    return 0;
}