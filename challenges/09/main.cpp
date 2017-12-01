#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

#include "input.h"

std::string discardIgnoredCharacters(const std::string & s) {
    std::string returnValue;
    auto it = s.begin();
    while(it != s.end()) {
        if(*it == '!') {
            ++it;
        }
        else {
            returnValue.push_back(*it);
        }
        ++it;
    }
    return returnValue;
}

auto removeGarbage(const std::string &s) {
    std::string returnValue;
    auto numberOfGarbageCharacters = 0u;
    auto it = s.begin();
    while (it != s.end()) {
        auto startGarbage = std::find(it, s.end(), '<');
        auto endGarbage = std::find(startGarbage, s.end(), '>');
        returnValue.append(it, startGarbage);
        it = endGarbage;

        auto garbageDistance = std::distance(startGarbage, endGarbage);
        if (garbageDistance >= 1) {
            numberOfGarbageCharacters += garbageDistance-1;
        }

        if(it != s.end()){
            ++it;
        }
    }
    return make_pair(returnValue, numberOfGarbageCharacters);
}

unsigned int getScore(const std::string & s) {
    auto level = 0u;
    auto score = 0u;
    for(auto c: s) {
        if (c == '{') {
            ++level;
            score += level;
        }
        if (c == '}') {
            --level;
        }
    }

    return score;
}

int main() {
    std::string input = input::readSingleLineFile("input/input09.txt");
    auto prunedData = removeGarbage(discardIgnoredCharacters(input));
    std::cout << getScore(prunedData.first) << " " << prunedData.second <<  "\n";
    return 0;
}