#include <assert.h>
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <set>

#include "algo.h"
#include "input.h"

enum class Op {
    INC, DEC
};

struct Instruction {

    explicit Instruction(const std::string & str) {
        auto words = input::split(str);
        assert(words.size() == 7);

        registerName = words[0];
        op = getOp(words[1]);
        offset = std::stoi(words[2]);

        assert(words[3] == "if");
        targetRegister = words[4];

        using namespace std::placeholders;
        condition = std::bind(getFunction(words[5]), _1, std::stoi(words[6]));
    }

    std::string registerName;
    Op op;
    int offset;
    std::string targetRegister;
    std::function<bool(int)> condition;

private:
    Op getOp(const std::string& opString) {
        if (opString == "inc") return Op::INC;
        if (opString == "dec") return Op::DEC;
        throw std::runtime_error("Invalid operation");
    }

    std::function<bool(int, int)> getFunction(const std::string& func) {
        if (func == ">") return std::greater<int>();
        if (func == ">=") return std::greater_equal<int>();
        if (func == "<") return std::less<int>();
        if (func == "<=") return std::less_equal<int>();
        if (func == "==") return std::equal_to<int>();
        if (func == "!=") return std::not_equal_to<int>();
        throw std::runtime_error("Invalid function");
    }
};


int applyOperation(int value, const Instruction& instruction) {
    if (instruction.op == Op::INC) {
        return value + instruction.offset;
    }
    return value - instruction.offset;
}

Instruction toInstruction(const std::string & s) {
    return Instruction(s);
}

auto populateRegisters(const auto& instructions) {
    std::set<std::string> registers;
    
    std::transform(instructions.begin(), instructions.end(), std::inserter(registers, registers.begin()), [](const Instruction& i) { return i.registerName; });
    std::transform(instructions.begin(), instructions.end(), std::inserter(registers, registers.begin()), [](const Instruction& i) { return i.targetRegister; });

    std::map<std::string, int> registerValues;
    std::transform(registers.begin(), registers.end(), std::inserter(registerValues, registerValues.begin()), [](const std::string & registerName) { return make_pair(registerName, 0);});
    return registerValues;
}

int main() { 
    const auto instructions = algo::map(input::readMultiLineFile("input/input08.txt"), toInstruction);
    auto registerValues = populateRegisters(instructions);
    
    auto highestValueSeen = 0;
    std::for_each(instructions.begin(), instructions.end(), [&registerValues, &highestValueSeen](const Instruction &i){
        if(i.condition(registerValues.at(i.targetRegister))) {
            registerValues[i.registerName] = applyOperation(registerValues.at(i.registerName), i);
            highestValueSeen = std::max(highestValueSeen, registerValues[i.registerName]);
        }
    });

    const auto max = std::max_element(registerValues.begin(), registerValues.end(), [](auto regPair1, auto regPair2) { return regPair1.second < regPair2.second;});
    std::cout << max->second << " " << max->first << "\n";
    std::cout << highestValueSeen << "\n";
    return 0;
}