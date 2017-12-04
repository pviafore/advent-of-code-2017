#include "input.h"
#include "algo.h"
#include "assert.h"

#include <algorithm>
#include <iostream>
#include <numeric>

auto getDifference(const std::vector<int>& numbers) {
    auto [min,max] = std::minmax_element(numbers.begin(), numbers.end());
    return *max - *min;
}

auto getEvenlyDivisible(const std::vector<int>& numbers) { 
    auto pairs = algo::find_matching_pairs(numbers, [](auto num1, auto num2) { return num1 % num2 == 0; });
    assert(pairs.size() == 1);
    return pairs[0].first / pairs[0].second;
}

auto getSumOfRowOperations(auto numbers, auto rowOperation){
    auto rowValues = algo::map(numbers, rowOperation);
    return std::accumulate(rowValues.begin(), rowValues.end(), 0u);
}

int main() {
    auto numbers = algo::map(input::readMultiLineFile("input/input02.txt"), input::toNumbers);
    std::cout << getSumOfRowOperations(numbers, getDifference) << "\n";
    std::cout << getSumOfRowOperations(numbers, getEvenlyDivisible) << "\n";
}