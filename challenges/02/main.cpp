#include "input.h"
#include "algo.h"

#include <algorithm>
#include <iostream>
#include <numeric>

auto getDifference(const std::vector<int>& numbers) {
    auto [min,max] = std::minmax_element(numbers.begin(), numbers.end());
    return *max - *min;
}

auto getEvenlyDivisible(const std::vector<int>& numbers) { 
    auto pairs = algo::find_pairs(numbers, [](auto num1, auto num2) { return num1 % num2 == 0; });
    return std::accumulate(pairs.begin(), pairs.end(), 0u, [](auto sum, auto p){ return sum + p.first / p.second; });
}

auto getSumOfRowOperations(auto numbers, auto rowOperation){
    std::vector<int> differences;
    std::transform(numbers.begin(), numbers.end(), std::back_inserter(differences), rowOperation);
    return std::accumulate(differences.begin(), differences.end(), 0u);
}

int main() {
    auto numbers = input::readMultiLineFile("input/input02.txt", input::toNumbers);
    std::cout << getSumOfRowOperations(numbers, getDifference) << "\n";
    std::cout << getSumOfRowOperations(numbers, getEvenlyDivisible) << "\n";
}