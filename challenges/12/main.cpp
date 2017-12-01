#include <assert.h>
#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "algo.h"
#include "input.h"

auto toPipeInformation(const std::string& input) {
    auto words = input::split(input);
    assert(words[1] == "<->");

    std::vector<std::string> connections(words.begin()+2, words.end());
    return std::make_pair(words[0], algo::map(connections, input::dropTrailingComma));
}

auto getNodesConnected(const auto& connections, const std::string& start) {
    std::set<std::string> nodesSeen;
    std::deque<std::string> nodesToCheck;
    nodesToCheck.push_front(start);

    while(!nodesToCheck.empty()) {
        const auto& node = nodesToCheck.front();
        nodesToCheck.pop_front();
        if(nodesSeen.find(node) == nodesSeen.end()) {
            const auto& nextConnections = connections.at(node);
            std::copy(nextConnections.begin(), nextConnections.end(), std::back_inserter(nodesToCheck));
        }
        nodesSeen.insert(node);
    }
    return nodesSeen;
}

auto getGroups(auto connections) {
    auto numberOfGroups = 0u;
    while(!connections.empty()) {
        auto group = getNodesConnected(connections, connections.begin()->first);
        ++numberOfGroups;
        auto alreadySeen = [&group](const auto& connection) { return group.find(connection.first) != group.end(); };
        algo::erase_if(connections, alreadySeen);
    }
    return numberOfGroups;
}

int main() {
    auto pipes = input::readMultiLineFile("input/input12.txt");
    std::map<std::string, std::vector<std::string>> connections;
    std::transform(pipes.begin(), pipes.end(), std::inserter(connections, connections.begin()), toPipeInformation);
    std::cout << getNodesConnected(connections, "0").size() << " " << getGroups(connections) << "\n";
    return 0;
}
