#include "input.h"

#include <algorithm>
#include <iostream>
#include <numeric>


auto convertToNumbers(const std::string & str) {
    auto v = input::split(str);
    std::vector<int> out;
    std::transform(v.begin(), v.end(), std::back_inserter(out), [](auto s){ return std::stoi(s); } );
    return out;
}

auto getDifference(const std::vector<int>& numbers) {
    auto [min,max] = std::minmax_element(numbers.begin(), numbers.end());
    return *max - *min;
}

auto getEvenlyDivisible(const std::vector<int>& numbers) { 
    for (auto num : numbers) {
        auto isEvenlyDivisible = [num](auto num2) { return num != num2 && num % num2 == 0; };
        auto match = std::find_if(numbers.begin(), numbers.end(), isEvenlyDivisible);
        if (match != numbers.end()) {
            return num / *match;
        }
    }
    throw std::runtime_error("No match found");
    
}

auto getSumOfRowOperations(const std::vector<std::string> & strings, auto rowOperation){
    std::vector<std::vector<int>> numbers;

    std::transform(strings.begin(), strings.end(), std::back_inserter(numbers), convertToNumbers);
    
    std::vector<int> differences;
    std::transform(numbers.begin(), numbers.end(), std::back_inserter(differences), rowOperation);

    return std::accumulate(differences.begin(), differences.end(), 0u);
}

int main() {
    auto strings = input::readMultiLineFile("input/input02.txt");
    std::cout << getSumOfRowOperations(strings, getDifference) << "\n";
    std::cout << getSumOfRowOperations(strings, getEvenlyDivisible) << "\n";
}