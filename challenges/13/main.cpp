#include <algorithm>
#include <functional>
#include <iostream>
#include <ostream>
#include <string>

#include "algo.h"
#include "input.h"

class Layer {
public:
    int depth;
    int range;

    Layer(int depth, int range) : depth(depth), range(range) {}

    void tick() {
        setNextDirectionAndPosition(direction, currentScan); 
    }

    bool isScanAtTop() const {
        return currentScan == 1;
    }

    int getScanPosition(int tick) const {
        auto total = 2*range - 2;
        auto next = (tick + depth) % total + 1;
        return next <= range ? next : 2*range - next;
    }

private: 
    int currentScan = 1;

    enum class Direction { UP, DOWN };
    Direction direction = Direction::DOWN;

    void setNextDirectionAndPosition(Direction& dir, int& position) const {
        if(position == 1) {
            dir = Direction::DOWN;
        }
        if(position == range) {
            dir = Direction::UP;
        }
        if(dir == Direction::UP)  {
            position--;
        }
        else {
            position++;
        }
    }
};

Layer toLayer(const std::string & str) {
    const auto colon = str.find(":");
    const auto depth = std::string(str.begin(), str.begin() + colon);
    const auto range = std::string(str.begin() + colon + 1, str.end());
    return Layer{stoi(depth), stoi(range)};
}

std::ostream& operator<<(std::ostream& os, const Layer& layer) {
    os << std::to_string(layer.depth) << " " << std::to_string(layer.range);
    return os;
}

int getSeverityOfViolations(auto layers) {
    auto it = layers.begin();
    auto deepestLayer = layers.rbegin()->depth;
    auto severity = 0;
    for(int i = 0; i <= deepestLayer; ++i) {
        if(i == it->depth) {
            if(it->isScanAtTop()) {
                severity += it->depth * it->range;
            }
            ++it;
        }
        std::for_each(layers.begin(), layers.end(), std::mem_fn(&Layer::tick));
    }
    return severity;
}

int getDelay(const auto& layers) {
    int delay = 1;
    while (std::any_of(layers.begin(), layers.end(), [delay](const Layer& l) { return l.getScanPosition(delay) == 1;})) {
        delay++;
    }
    return delay;
}


int main() {
    auto layers = algo::map(input::readMultiLineFile("input/input13.txt"), toLayer);
    auto severity = getSeverityOfViolations(layers);
    auto delay = getDelay(layers);
    std::cout << severity << " " << delay << "\n";
    return 0;
}