#include <cassert>
#include <iostream>
#include <regex>
#include <set>
#include <span>
#include <string>
#include <unordered_map>

#include "input.h"


enum class Direction {
    Left,
    Right
};

struct Action {
    char toWrite = '0';
    Direction direction = Direction::Left;
    char nextState = 'A';
};

class TuringMachine {
public: 
    TuringMachine(std::span<std::string> instructions){
        startingState = *(instructions[0].rbegin() + 1); 

        std::regex numberOfStepsRegex{ "Perform a diagnostic checksum after (\\d+) steps."};
        std::smatch match;
        auto result = std::regex_search(instructions[1], match, numberOfStepsRegex);
        assert(result);

        numberOfSteps = std::stoul(match[1]);
        
        for(size_t lineIndex = 2; lineIndex < instructions.size(); lineIndex += 9) {
            char currentState = *(instructions[lineIndex].rbegin() + 1);

            // assuming value zero is always first
            char ifZeroValue = *(instructions[lineIndex+2].rbegin() + 1);
            Direction ifZeroDirection = (instructions[lineIndex+3].find("left") != std::string::npos) ? Direction::Left : Direction::Right;
            char ifZeroNextState = *(instructions[lineIndex+4].rbegin() + 1);

            char ifOneValue = *(instructions[lineIndex+6].rbegin() + 1);
            Direction ifOneDirection = (instructions[lineIndex+7].find("left") != std::string::npos) ? Direction::Left : Direction::Right;
            char ifOneNextState = *(instructions[lineIndex+8].rbegin() + 1);

            stateToActionMapping[currentState] = std::pair{Action{ifZeroValue, ifZeroDirection, ifZeroNextState}, Action{ifOneValue, ifOneDirection, ifOneNextState}};
        }
    }

    void run() { 
        char state = startingState;
        int position = 0;
        for(unsigned int counter=0;  counter<numberOfSteps; ++counter){
            auto [ifZero, ifOne] = stateToActionMapping[state];
            auto action = (oneValues.find(position) == oneValues.end()) ? ifZero : ifOne;
            if(action.toWrite == '1'){
                oneValues.insert(position);
            }
            else {
                oneValues.erase(position);
            }
            position += (action.direction == Direction::Left) ? -1 : 1;
            state = action.nextState;
        }
    }

    size_t getNumberOfOnes() const {
        return oneValues.size();
    }


private:
    char startingState;
    unsigned long numberOfSteps;
    std::unordered_map<char, std::pair<Action, Action>> stateToActionMapping;
    std::set<int> oneValues;
};

int main() {
    auto instructions = input::readMultiLineFile("input/input25.txt");
    TuringMachine turingMachine(instructions);
    turingMachine.run();
    std::cout << "Diagnostic Checksum: " << turingMachine.getNumberOfOnes() << "\n";
    return 0;
}