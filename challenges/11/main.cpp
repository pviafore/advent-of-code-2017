#include <iostream>
#include <map>
#include <numeric>

#include "algo.h"
#include "input.h"


using Coordinate = std::pair<int,int>;

auto toCoordinate(const std::string &direction) {
    std::map<std::string, Coordinate> m  = {
        {"n", {0,10}},
        {"ne", {5,5}},
        {"se", {5,-5}},
        {"s", {0,-10}},
        {"sw", {-5,-5}},
        {"nw", {-5, 5}} 
    };
    return m.at(direction);
}

std::vector<Coordinate> addCoordinates(std::vector<Coordinate> path, const Coordinate& coord2) {
    auto newLocation =  Coordinate { path.rbegin()->first + coord2.first, path.rbegin()->second + coord2.second};
    path.push_back(newLocation);
    return path;
}

auto findShortestPath(const Coordinate& childLocation) {
    auto tempPosition = Coordinate(abs(childLocation.first), abs(childLocation.second));
    auto numSteps = 0u;
    if(tempPosition.first < tempPosition.second) {
        while(tempPosition.first != 0) {
            tempPosition.first -= 5;
            tempPosition.second -= 5;
            numSteps++;
        }
        numSteps += tempPosition.second/10;

    }
    else {
        while(tempPosition.second >= 10) {
            tempPosition.first -= 5;
            tempPosition.second -= 5;
            numSteps++;
        }
        numSteps += tempPosition.first/5;
    }
    return numSteps;
}

auto moveChild(const auto& directions) {
    std::vector<Coordinate> path = {Coordinate{0,0}};
    auto childPath = std::accumulate(directions.begin(), directions.end(), path, addCoordinates);
    auto shortestPaths = algo::map(childPath, findShortestPath);
    auto furthestAway = std::max_element(shortestPaths.begin(), shortestPaths.end());
    return std::make_pair(*shortestPaths.rbegin(), *furthestAway);
}

int main() {

    auto input = input::split(input::readSingleLineFile("input/input11.txt"), ',');
    auto directions = algo::map(input, toCoordinate);

    auto shortestPath = moveChild(directions);

    std::cout << shortestPath.first << " " << shortestPath.second << "\n";

    return 0;
}