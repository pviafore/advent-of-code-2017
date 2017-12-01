#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "input.h"
#include "algo.h"
using Rule = std::pair<std::string, std::string>;

Rule toRule(const std::string& text) {
    auto parts = input::split(text, ' ');
    assert(parts.size() == 3);
    return {parts[0], parts[2]};
}


using Blocks = std::vector<std::string>;
Blocks getBlocks(const std::string & text) {
    if (text.length() == 11) {
        return {text};
    }
    else {
        // 4x4 - split into 4 2 by 2s
        assert(text.length() == 19);
        //           111111111
        // 0123456789012345678
        // xxxx/xxxx/xxxx/xxxx
        std::string block1 = std::string(text.c_str(), text.c_str()+2) + "/" + std::string(text.c_str()+5, text.c_str()+7);
        std::string block2 = std::string(text.c_str()+2, text.c_str()+4) + "/" + std::string(text.c_str()+7, text.c_str()+9);
        std::string block3 = std::string(text.c_str()+10, text.c_str()+12) + "/" + std::string(text.c_str()+15, text.c_str()+17);
        std::string block4 = std::string(text.c_str()+12, text.c_str()+14) + "/" + std::string(text.c_str()+17, text.c_str()+19);

        return {block1, block2, block3, block4};
    }
}

std::string transpose(const std::string& text){
    auto splitted = input::split(text, '/');
    assert(splitted.size() == 2 || splitted.size() == 3);
    std::vector<std::string> out;
    for(unsigned int i = 0; i<splitted.size(); ++i){
        std::string transposed;
        for(const auto& s: splitted){
            transposed.push_back(s[i]);
        }
        out.emplace_back(transposed.rbegin(), transposed.rend());
    }
    return input::join(out, "/");
}

std::string flip(const std::string& text) {
    auto splitted = input::split(text, '/');
    std::vector<std::string> out;
    assert(splitted.size() == 2 || splitted.size() == 3);
    for(const auto& toBeFlipped: splitted){
        out.emplace_back(toBeFlipped.rbegin(), toBeFlipped.rend());
    }
    return input::join(out, "/");
}

using RulesMapping = std::map<std::string, Blocks>;

RulesMapping createRuleMapping(std::span<Rule> rules){
    RulesMapping out;

    for(const auto& rule: rules) {
        auto blocks = getBlocks(rule.second);
        out[rule.first] = blocks;
        out[transpose(rule.first)] = blocks;
        out[transpose(transpose(rule.first))] = blocks;
        out[transpose(transpose(transpose(rule.first)))] = blocks;
        out[flip(rule.first)] = blocks;
        out[transpose(flip(rule.first))] = blocks;
        out[transpose(transpose(flip(rule.first)))] = blocks;
        out[transpose(transpose(transpose(flip(rule.first))))] = blocks;
    }

    return out;
}

Blocks recombine(const Blocks& pattern) {
    auto subblockSize = std::distance(pattern.begin()->begin(), pattern.begin()->begin() + pattern.begin()->find("/"));
    auto bigBlockSize = static_cast<int>(sqrt(pattern.size()) * subblockSize);
    if (bigBlockSize % 6 != 0 || subblockSize == 2){
        return pattern;
    }
    // we know each group of six characters across and 6 blocks down will produce 9 2x2 blocks
    // so, let's do the top 2, middle 2, and bottom 2 in a row, six at a top , and then move on.

    auto blocksPerRow = bigBlockSize / subblockSize;
    Blocks out;
    for(unsigned int index=0; index < pattern.size(); index += 2*blocksPerRow) {
        std::string top1;
        std::string bottom1;
        std::string top2;
        std::string bottom2;
        std::string top3;
        std::string bottom3;

        for(unsigned int blockIndex=0; blockIndex < blocksPerRow; ++blockIndex){
            auto blockPattern = input::split(pattern[index + blockIndex],'/');
            assert(blockPattern.size() == 3);
            top1 += blockPattern[0];       
            bottom1 += blockPattern[1];       
            top2 += blockPattern[2];       
            
            auto blockPattern2 = input::split(pattern[index + blocksPerRow + blockIndex],'/');
            assert(blockPattern2.size() == 3);
            bottom2 += blockPattern2[0];
            top3 += blockPattern2[1];
            bottom3 += blockPattern2[2];
        }

        for(size_t counter = 0; counter < top1.size(); counter+=2) {
            std::string block = std::string{top1.begin()+counter, top1.begin()+counter+2} + "/" + std::string{bottom1.begin() + counter, bottom1.begin()+counter+2};
            out.push_back(block);
        }
        for(size_t counter = 0; counter < top2.size(); counter+=2) {
            std::string block = std::string{top2.begin()+counter, top2.begin()+counter+2} + "/" + std::string{bottom2.begin() + counter, bottom2.begin()+counter+2};
            out.push_back(block);
        }
        for(size_t counter = 0; counter < top3.size(); counter+=2) {
            std::string block = std::string{top3.begin()+counter, top3.begin()+counter+2} + "/" + std::string{bottom3.begin() + counter, bottom3.begin()+counter+2};
            out.push_back(block);
        }

    }

    return out;
}


Blocks transform(const Blocks& pattern, const RulesMapping& rulesMapping) {
    Blocks out;
    auto subblockSize = std::distance(pattern.begin()->begin(), pattern.begin()->begin() + pattern.begin()->find("/"));
    auto bigBlockSize = static_cast<int>(sqrt(pattern.size()) * subblockSize);
    auto blocksPerRow = bigBlockSize / subblockSize;

    for(size_t rowIndex=0; rowIndex<pattern.size(); rowIndex += blocksPerRow){
        Blocks topRow;
        Blocks bottomRow;
        for(int blockIndex=0; blockIndex < blocksPerRow; ++blockIndex){

            auto newBlocks = rulesMapping.at(pattern[rowIndex+blockIndex]);
            topRow.push_back(newBlocks[0]);
            if(newBlocks.size() == 4) {
                topRow.push_back(newBlocks[1]);
                bottomRow.push_back(newBlocks[2]);
                bottomRow.push_back(newBlocks[3]);
            }
        }
        out.insert(out.end(), topRow.begin(), topRow.end());
        out.insert(out.end(), bottomRow.begin(), bottomRow.end());
    }
    return recombine(out);
}

Blocks iterate(const Blocks& pattern, const RulesMapping& rulesMapping, unsigned int times) {
    Blocks toBeChanged(pattern);
    for(unsigned int i=0; i < times; ++i){
        toBeChanged = transform(toBeChanged, rulesMapping);
    }
    return toBeChanged;
}

unsigned int getLightsOn(const Blocks& blocks){
    return std::accumulate(blocks.begin(), blocks.end(), 0U, [](unsigned int sum, const std::string& s) {
        return sum + std::ranges::count(s, '#');
    });
}

int main() { 
    Blocks startingPattern {".#./..#/###"};
    auto rules = algo::map(input::readMultiLineFile("input/input21.txt"), toRule);
    auto ruleMapping = createRuleMapping(rules);
    auto blocks = iterate(startingPattern, ruleMapping, 5U);
    std::cout << getLightsOn(blocks) << "\n";
    auto blocks18 = iterate(startingPattern, ruleMapping, 18U);
    std::cout << getLightsOn(blocks18) << "\n";
    return 0;
}