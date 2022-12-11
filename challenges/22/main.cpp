#include <compare>
#include <iostream>
#include <set>
#include <span>

#include "input.h"

enum class Direction {
    Up,
    Right,
    Down,
    Left
};

Direction turnRight(Direction dir){
    switch(dir){
        case Direction::Up: return Direction::Right;
        case Direction::Right: return Direction::Down;
        case Direction::Down: return Direction::Left;
        default:
        case Direction::Left: return Direction::Up;
        
    }
}
Direction turnLeft(Direction dir){
    switch(dir){
        case Direction::Up: return Direction::Left;
        case Direction::Left: return Direction::Down;
        case Direction::Down: return Direction::Right;
        default:
        case Direction::Right: return Direction::Up;
        
    }
}
Direction reverse(Direction dir){
    switch(dir){
        case Direction::Up: return Direction::Down;
        case Direction::Left: return Direction::Right;
        case Direction::Down: return Direction::Up;
        default:
        case Direction::Right: return Direction::Left;
        
    }
}
struct Coord { 
    long xPos;
    long yPos;

    std::strong_ordering operator<=>(const Coord& rhs) const = default;
};

Coord move(const Coord& coord, Direction dir) {
    switch(dir){
        case Direction::Up: return Coord{coord.xPos, coord.yPos-1};
        case Direction::Left: return Coord{coord.xPos - 1, coord.yPos};
        case Direction::Down: return Coord{coord.xPos, coord.yPos + 1};
        default:
        case Direction::Right: return Coord{coord.xPos + 1, coord.yPos};

    }
}

auto getInfected(std::span<std::string> grid) {
    std::set<Coord> infected;

    for(size_t rowIndex=0; rowIndex < grid.size(); ++rowIndex){
        for(size_t columnIndex=0; columnIndex < grid[rowIndex].size(); ++columnIndex){
            if(grid[rowIndex][columnIndex] == '#'){
                infected.insert(Coord{static_cast<int>(columnIndex), static_cast<int>(rowIndex)});
            }
        }
    }
    return infected;

}

size_t getActiveBursts(std::span<std::string> grid, unsigned int numberOfMoves){
    int midy = grid.size() / 2;
    int midx = grid[midy].length() / 2;

    std::set<Coord> infected = getInfected(grid);

    Direction virusDirection = Direction::Up;
    Coord virusPosition {midx, midy};
    size_t numberOfActiveBursts = 0;

    for(unsigned int iterations=0; iterations < numberOfMoves; ++iterations){
        bool isInfected = infected.find(virusPosition) != infected.end();
        virusDirection = (isInfected) ? turnRight(virusDirection) : turnLeft(virusDirection);
        if(isInfected) {
            infected.erase(virusPosition);
        }
        else{
            numberOfActiveBursts++;
            infected.insert(virusPosition);
        }
        virusPosition = move(virusPosition, virusDirection);
    }

    return numberOfActiveBursts;

}

size_t getAdvancedNumberOfBursts(std::span<std::string> grid, unsigned int numberOfMoves){
    int midy = grid.size() / 2;
    int midx = grid[midy].length() / 2;

    std::set<Coord> infected = getInfected(grid);
    std::set<Coord> weakened;
    std::set<Coord> flagged;

    Direction virusDirection = Direction::Up;
    Coord virusPosition {midx, midy};
    size_t numberOfActiveBursts = 0;

    for(unsigned int iterations=0; iterations < numberOfMoves; ++iterations){
        bool isInfected = infected.find(virusPosition) != infected.end();
        bool isWeakened = weakened.find(virusPosition) != weakened.end();
        bool isFlagged = flagged.find(virusPosition) != flagged.end();

        if(!isInfected && !isWeakened && !isFlagged){
            virusDirection = turnLeft(virusDirection);
            weakened.insert(virusPosition);
        }
        else if(isWeakened) {
            numberOfActiveBursts++;
            weakened.erase(virusPosition);
            infected.insert(virusPosition);
        }
        else if(isInfected) { 
            virusDirection = turnRight(virusDirection);
            infected.erase(virusPosition);
            flagged.insert(virusPosition);
        }
        else if(isFlagged) {
            virusDirection = reverse(virusDirection);
            flagged.erase(virusPosition);
        }

        virusPosition = move(virusPosition, virusDirection);

    }

    return numberOfActiveBursts;
     
}

int main() {
    auto grid = input::readMultiLineFile("input/input22.txt");
    std::cout << "Active bursts: " << getActiveBursts(grid, 10'000) << "\n";
    std::cout << "Active bursts: " << getAdvancedNumberOfBursts(grid, 10'000'000) << "\n";
}