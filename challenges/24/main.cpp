#include <algorithm>
#include <iostream>
#include <queue>
#include <ranges>
#include <span>
#include <string>
#include <tuple>
#include <vector>

#include "algo.h"
#include "input.h"

struct Connector {
    unsigned long side1;
    unsigned long side2;

    unsigned int getStrength() const { 
        return side1 + side2;
    }

    std::strong_ordering operator<=>(const Connector&) const = default;
};

Connector toConnector(const std::string& text) { 
    auto slash = text.find("/");
    std::string side1 = text.substr(0, slash);
    std::string side2 = text.substr(slash+1, std::string::npos);
    return {std::stoul(side1), std::stoul(side2)};
}

// bridge, strength, remaining
using Candidate = std::tuple<unsigned long, unsigned long, std::vector<Connector>>;

unsigned long getStrongestBridge(std::span<Connector> connectors) {

    auto isCandidateLessThan = [](const auto& candidate1, const auto& candidate2){
        return std::get<1>(candidate1) < std::get<1>(candidate2);
    };
 
    std::priority_queue<Candidate, std::vector<Candidate>, decltype(isCandidateLessThan)> candidates;
    candidates.push(Candidate{0, 0, {connectors.begin(), connectors.end()}});
    unsigned long maxStrength = 0;
    while(!candidates.empty()) {
        auto [bridge, strength, remaining] = candidates.top();
        candidates.pop();

        auto matches = remaining | std::views::filter([bridge](const auto& c){
            return c.side1 == bridge || c.side2 == bridge;
        });
        
        for(const auto& match : matches){
            std::vector<Connector> newRemaining{remaining};
            newRemaining.erase(std::ranges::find(newRemaining, match));
            unsigned long newBridge = (bridge == match.side1) ? match.side2 : match.side1;
            unsigned long newStrength = strength + match.getStrength();
            maxStrength = std::max(maxStrength, newStrength);
            candidates.emplace(newBridge, newStrength, newRemaining);
        }
    }

    return maxStrength;
}

using NewCandidate = std::tuple<unsigned long, unsigned long, unsigned long, std::vector<Connector>>;

unsigned long getLongestStrongestBridge(std::span<Connector> connectors) {

    auto isCandidateLessThan = [](const auto& candidate1, const auto& candidate2){
        return std::get<1>(candidate1) < std::get<1>(candidate2);
    };
 
    std::priority_queue<NewCandidate, std::vector<NewCandidate>, decltype(isCandidateLessThan)> candidates;
    candidates.push(NewCandidate{0, 0, 0, {connectors.begin(), connectors.end()}});
    std::pair<unsigned long, unsigned long> longestAndStrongest {0, 0};
    while(!candidates.empty()) {
        auto [bridge, length, strength, remaining] = candidates.top();
        candidates.pop();

        auto matches = remaining | std::views::filter([bridge](const auto& c){
            return c.side1 == bridge || c.side2 == bridge;
        });
        
        for(const auto& match : matches){
            std::vector<Connector> newRemaining{remaining};
            newRemaining.erase(std::ranges::find(newRemaining, match));
            unsigned long newBridge = (bridge == match.side1) ? match.side2 : match.side1;
            unsigned long newStrength = strength + match.getStrength();
            longestAndStrongest = std::max(longestAndStrongest, std::make_pair(length + 1, newStrength));
            candidates.emplace(newBridge, length + 1, newStrength, newRemaining);
        }
    }

    return longestAndStrongest.second;
}


int main() {
    auto connectors = algo::map(input::readMultiLineFile("input/input24.txt"), toConnector);
    std::cout << getStrongestBridge(connectors) << "\n";
    std::cout << getLongestStrongestBridge(connectors) << "\n";
}