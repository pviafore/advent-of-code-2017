#include <algorithm>
#include <iostream>
#include <string>

#include "algo.h"
#include "input.h"

std::string sortString(std::string s) {
    std::sort(s.begin(), s.end());
    return s;
}

bool isUnique(const std::string& passphrase) {
    auto words = algo::map(input::split(passphrase), sortString);
    std::sort(words.begin(), words.end());
    return words.end() == std::unique(words.begin(), words.end());
}

int main() {
    auto passPhrases = input::readMultiLineFile("input/input04.txt");
    auto count = std::count_if(passPhrases.begin(), passPhrases.end(), isUnique);
    std::cout << count << "\n";
    return 0;
}