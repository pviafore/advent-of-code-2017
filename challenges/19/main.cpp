#include <assert.h>
#include <iostream>

#include <input.h>

struct Point {
    size_t x = 0;
    size_t y = 0;

    bool operator!=(const Point& rhs) const{
        return this->x != rhs.x || this->y != rhs.y;
    }
};

enum class Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

Direction inverse(Direction d){
    switch (d){
        case Direction::UP: return Direction::DOWN;
        case Direction::DOWN: return Direction::UP;
        case Direction::LEFT: return Direction::RIGHT;
        case Direction::RIGHT: return Direction::LEFT;
    }
    throw std::runtime_error("Missing direction");
}

struct Intersection {
    Point point;
    Direction directions[2];
    char symbol;
};

std::vector<Direction> get_neighbors(size_t i, size_t j, const std::vector<std::string> &lines){
    std::vector<Direction> directions;
    if (i != 0 && j < lines[i-1].size() ){
        auto above = lines[i-1][j];
        if (above != '-' && above != ' '){
            directions.push_back(Direction::UP);
        }
    }
    if (i != lines.size()-1 && j < lines[i+1].size()){
        auto below = lines[i+1][j];
        if (below != '-' && below != ' '){
            directions.push_back(Direction::DOWN);
        }
    }
    if (j != 0){
        auto left = lines[i][j-1];
        if (left != '|' && left != ' '){
            directions.push_back(Direction::LEFT);
        }
    }
    if (j != lines[i].size() -1){
        auto right = lines[i][j+1];
        if (right != '|' && right != ' '){
            directions.push_back(Direction::RIGHT);
        }
    }
    assert(directions.size() < 3);
    assert(directions.size() > 0);
    return directions;
}

class Diagram {
public:
    explicit Diagram(const std::vector<std::string>& lines): start{lines[0].find('|'),0} {
        for(size_t i = 0; i < lines.size(); ++i){
            for(size_t j = 0; j < lines[i].size(); ++j){
                char letter = lines[i][j];
                if(letter != ' ' && letter != '|' && letter != '-' ){
                    const auto neighbors = get_neighbors(i, j, lines);
                    if (neighbors.size() == 1) {
                        end = {j, i};
                        intersections.push_back(Intersection{end, {neighbors[0], neighbors[0]}, letter});
                    }
                    else {
                        intersections.push_back(Intersection{{j, i}, {neighbors[0], neighbors[1]}, letter});
                    }
                }
            }
        }
    }

    std::pair<std::string, unsigned int> getString() const {
        std::string out;
        unsigned int steps = 1u;
        Point pos = start;
        Direction dir = Direction::DOWN;
        while (pos != end){
            Intersection next;
            std::vector<Intersection> intersectionsInArea;
            switch (dir){
                case Direction::DOWN:
                    std::copy_if(intersections.begin(), intersections.end(), std::back_inserter(intersectionsInArea), [pos](const auto& i){
                        return i.point.x == pos.x && i.point.y > pos.y;
                    });
                    next = *intersectionsInArea.begin();
                    steps += (next.point.y - pos.y);
                    break;
                case Direction::UP:
                    std::copy_if(intersections.begin(), intersections.end(), std::back_inserter(intersectionsInArea), [pos](const auto& i){
                        return i.point.x == pos.x && i.point.y < pos.y;
                    });
                    next = *intersectionsInArea.rbegin();
                    steps += (pos.y - next.point.y);
                    break;
                case Direction::LEFT:
                    std::copy_if(intersections.begin(), intersections.end(), std::back_inserter(intersectionsInArea), [pos](const auto& i){
                        return i.point.x <  pos.x && i.point.y == pos.y;
                    });
                    next = *intersectionsInArea.rbegin();
                    steps += (pos.x - next.point.x);
                    break;
                case Direction::RIGHT:
                    std::copy_if(intersections.begin(), intersections.end(), std::back_inserter(intersectionsInArea), [pos](const auto& i){
                        return i.point.x > pos.x && i.point.y == pos.y;
                    });
                    next = *intersectionsInArea.begin();
                    steps += (next.point.x - pos.x);
                    break;
            }

            pos = next.point;
            dir = (dir == inverse(next.directions[0])) ? next.directions[1] : next.directions[0];
            if (next.symbol != '+') {
                out += std::string(1, next.symbol);
            }
        }
        return {out, steps};
    }
private:
    std::vector<Intersection> intersections;
    Point start;
    Point end;
};

int main() {
    Diagram diagram(input::readMultiLineFile("input/input19.txt"));
    auto [string, steps] = diagram.getString();
    std::cout << "String of route: " << string << "\n";
    std::cout << "# of steps: " << steps << "\n";

}
