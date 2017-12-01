#include <assert.h>
#include <cmath>
#include <iostream>
#include <iterator>
#include <ostream>
#include <optional>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "algo.h"
#include "input.h"

std::string stripFirstAndLastCharacter(const std::string &s) {
    return std::string(s.begin()+1, s.end()-1);
}


struct InputTower {
    explicit InputTower(const std::string &input) : totalWeight(std::nullopt) {
        auto words = input::split(input);
        name = words[0];

        weight = std::stoi(stripFirstAndLastCharacter(words[1]));

        if(words.size() > 3) {
            std::transform(words.begin()+3, words.end(), std::back_inserter(subTowers), input::dropTrailingComma);
        }

    }

    bool isSupporting(const std::string &towerName) const {
        return std::find(subTowers.begin(), subTowers.end(), towerName) != subTowers.end();
    }

    unsigned int getTotalWeight() const {
        return totalWeight.value_or(0u);
    }

    std::string name;
    unsigned int weight;
    std::vector<std::string> subTowers;
    std::optional<unsigned int> totalWeight;
};

auto findSupportingTower(auto towers, const std::string& towerName) {
    auto match = std::find_if(towers.begin(), towers.end(), [towerName](const auto &towerPair) { return towerPair.second.isSupporting(towerName);});
    auto supportingTower = (match == towers.end()) ? std::nullopt : std::optional<std::string>(match->first);
    return supportingTower;
}

std::string findBottomTowerName(auto towers) {
    auto isTopTower = [](const auto & tower) { return tower.second.subTowers.empty(); };
    decltype(towers) topTowers;
    std::copy_if(towers.begin(), towers.end(), std::inserter(topTowers, topTowers.begin()), isTopTower);

    auto currentTowerIter = topTowers.begin();
    auto currentTower = currentTowerIter->first;
    auto supportingTower = findSupportingTower(towers, currentTower);
    while(supportingTower.has_value()) {
        currentTower = supportingTower.value();
        supportingTower = findSupportingTower(towers, currentTower);
    }

    return currentTower;
}

void populateTotalWeights(auto& towerLookup, const std::string& bottomTowerName) {
    auto &tower = towerLookup.at(bottomTowerName);
    std::for_each(tower.subTowers.begin(), tower.subTowers.end(), [&towerLookup](const std::string& subTower) {
        populateTotalWeights(towerLookup, subTower);
    });
    tower.totalWeight = tower.weight + std::accumulate(tower.subTowers.begin(), tower.subTowers.end(), 0u, [&towerLookup](auto sum, const std::string& s) { return sum + towerLookup.at(s).getTotalWeight(); });
}

std::string findImbalancedTower(const auto & towerLookup, const std::string& baseTowerName) {
    const auto &currentTower = towerLookup.at(baseTowerName);
    auto subTowers = algo::map(currentTower.subTowers, [&towerLookup](const std::string& s) { return towerLookup.at(s);});

    if(subTowers.size() == 1) {
        return findImbalancedTower(towerLookup, subTowers[0].name);
    }
    if(std::all_of(subTowers.begin(), subTowers.end(), [weight = subTowers[0].totalWeight](const InputTower& t) { return t.totalWeight == weight; })){
        return currentTower.name;
    }

    bool firstTwoTowersHaveEqualWeight = subTowers[0].totalWeight == subTowers[1].totalWeight;
    if (firstTwoTowersHaveEqualWeight) {
        auto inequal = std::find_if(subTowers.begin(), subTowers.end(), [expectedWeight = subTowers[0].totalWeight](const InputTower& tower) { return tower.totalWeight != expectedWeight;});
        assert(inequal != subTowers.end());
        return findImbalancedTower(towerLookup, inequal->name);
    }
    return (subTowers[0].totalWeight == subTowers[2].totalWeight) ? findImbalancedTower(towerLookup, subTowers[1].name) : findImbalancedTower(towerLookup, subTowers[0].name);
}

auto createTowerLookup(const auto & towers) {
    std::map<std::string, InputTower> towerLookup;
    std::transform(towers.begin(), towers.end(), std::inserter(towerLookup, towerLookup.begin()), [](const InputTower& tower) { return make_pair(tower.name, tower); });
    return towerLookup;
}

auto findDifference(const auto &towerLookup, const std::string & imbalancedTower) {
    auto upperTower = std::find_if(towerLookup.begin(), towerLookup.end(), [imbalancedTower](const auto& tower) { return tower.second.isSupporting(imbalancedTower);});
    auto subTowers = algo::map(upperTower->second.subTowers, [&towerLookup](const std::string& s) { return towerLookup.at(s);});
    auto wrongWeight = towerLookup.at(imbalancedTower).getTotalWeight();
    auto differenceIter = std::find_if(subTowers.begin(), subTowers.end(), [wrongWeight](const auto& tower) { return tower.totalWeight.value_or(0u) != wrongWeight;});
    return towerLookup.at(imbalancedTower).weight + (differenceIter->getTotalWeight() - wrongWeight);
}

int main () {
    auto towers = algo::map(input::readMultiLineFile("input/input07.txt"), [](const std::string& s){return InputTower(s);});
    auto towerLookup = createTowerLookup(towers);
    auto bottomTower = findBottomTowerName(towerLookup);
    std::cout <<  bottomTower << "\n";

    populateTotalWeights(towerLookup, bottomTower);
    auto imbalancedTower = findImbalancedTower(towerLookup, bottomTower);
    auto difference = findDifference(towerLookup, imbalancedTower);
    std::cout << imbalancedTower << " " << difference <<"\n";

    return 0;
}
