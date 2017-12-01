#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <tuple>
#include <vector>

using Position = std::pair<int, int>;

auto getWidth(unsigned int level) {
    return 2 * level + 1;
}

auto getTargetLevel(unsigned int target) { 
    auto level = 1u;
    auto ret = std::make_pair(1u, 1u);
    while(target > ret.second)
    {
        const auto numbers = 4*getWidth(level) - 4;
        ret = std::make_pair(ret.second+1, ret.second+numbers);
        ++level;
    }
    return std::make_tuple(level, ret.first, ret.second);
}

auto getNumberOfSteps(unsigned int target){
    const auto [level, min, max] = getTargetLevel(target);
    const auto width = getWidth(level);

    auto distanceToMidpoint = [target, level](auto n1, auto n2) { return abs((n1 + n2)/2 - target) + level; };
    
    const auto lowerLeft = (max - width + 1 ); 
    if (target >= lowerLeft) {
        return distanceToMidpoint(max, lowerLeft);
    }

    const auto upperLeft = (lowerLeft - width + 1);
    if (target >= upperLeft) {
        return distanceToMidpoint(upperLeft, lowerLeft);
    }

    const auto upperRight = (upperLeft - width + 1);
    if (target >= upperRight) {
        return distanceToMidpoint(upperRight, upperLeft);
    }
    
    return distanceToMidpoint(min-1, upperRight);     
}

Position addPairs(Position p1, Position p2) {
    return Position(p1.first + p2.first, p1.second + p2.second);
}

class LookupCache {
public:
    explicit LookupCache(unsigned int target) : target(target) {
        cache[Position(0, 0)] = 1;
    }

    auto calculate(auto p) {
        auto addPairLookup = [p, this](auto sum, auto m){ return sum + lookup(addPairs(p,m));};
        const auto sum = std::accumulate(modifiers.begin(), modifiers.end(), 0u, addPairLookup);
        if (sum > target && !answer.has_value()) {
            answer = sum;
        }
        cache[p] = sum;
        return sum;
    }

    auto hasReachedTarget() const {
        return answer.has_value();
    }

    auto getAnswer() const {
        return answer.value_or(0u);
    }
    
private:

    unsigned int lookup(auto p) const{
        const auto match = cache.find(p);
        return (match == cache.end()) ? 0 : match->second;
    }

    std::map<Position, unsigned int> cache;
    const unsigned int target;
    std::optional<unsigned int> answer;
    const std::vector<Position> modifiers = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };
};

class PositionIterator {
public:
    explicit PositionIterator(LookupCache& lookupCache) : lookupCache(lookupCache) {}

    enum class Direction {
        RIGHT, UP, LEFT, DOWN
    };

    void updatePosition(Direction direction) {
        switch (direction) {
            case Direction::RIGHT: position.first++; break;
            case Direction::UP: position.second--; break;
            case Direction::LEFT: position.first--; break;
            case Direction::DOWN: position.second++; break;
        }
    } 

    void move(Direction direction, unsigned int numberOfTimes=1) {
        for(unsigned int i = 0u; i < numberOfTimes; ++i){
            updatePosition(direction);
            lookupCache.calculate(position);
        }
    }

private:
    LookupCache & lookupCache;
    Position position = Position(0,0);
};

auto findNextLargestCumulativeValueWritten(unsigned int target) {
    LookupCache lookupCache(target);
    PositionIterator position(lookupCache);

    auto level = 1u;
    while(!lookupCache.hasReachedTarget()) {
        const auto width = getWidth(level);
        position.move(PositionIterator::Direction::RIGHT);
        position.move(PositionIterator::Direction::UP, width - 2);
        position.move(PositionIterator::Direction::LEFT, width - 1);
        position.move(PositionIterator::Direction::DOWN, width - 1);
        position.move(PositionIterator::Direction::RIGHT, width - 1);
        level++;
    }

    return lookupCache.getAnswer();
}

int main() {
    auto steps = getNumberOfSteps(325489);
    std::cout << steps << "\n";

    auto answer = findNextLargestCumulativeValueWritten(325489);
    std::cout << answer << "\n";
    
    return 0;
}