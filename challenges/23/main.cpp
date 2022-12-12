#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

#include "algo.h"
#include "input.h"

struct Instruction {
    std::string operation;
    std::string arg1;
    std::string arg2;
};

Instruction toInstruction(const std::string& str) { 
    auto text = input::split(str, ' ');
    assert(text.size() == 3);
    return {text[0], text[1], text[2]};
}

class Computer {
public:
    Computer(std::vector<Instruction> instructions): instructions(std::move(instructions)) {}

    void run() {
        while(instructionPointer >= 0 && static_cast<size_t>(instructionPointer) < instructions.size()){
            execute(instructions[instructionPointer]);
        }
    }
    
    unsigned int getMultiplicationsInvoked() const { 
        return numberOfMultiplicationsInvoked;
    }

private:
    std::vector<Instruction> instructions;
    unsigned int numberOfMultiplicationsInvoked = 0;
    int instructionPointer = 0;
    std::array<int, 8> registers { 0, 0, 0, 0, 0, 0, 0, 0};

    void execute(const Instruction& instruction){
        if(instruction.operation == "set") { set(instruction.arg1, instruction.arg2); ++instructionPointer; }
        else if(instruction.operation == "jnz") { jnz(instruction.arg1, instruction.arg2); }
        else if(instruction.operation == "sub") { sub(instruction.arg1, instruction.arg2); ++instructionPointer; }
        else if(instruction.operation == "sub") { mul(instruction.arg1, instruction.arg2); ++instructionPointer; }
    }

    int getRegisterOrValue(const std::string& arg) const{
        return (arg[0] >= 'a' && arg[0] <= 'h') ? getRegisterValue(arg) : std::stoi(arg);
    }

    void set(const std::string& arg1, const::std::string& arg2){
        registers[getRegisterIndex(arg1)] = getRegisterOrValue(arg2);
    }
    
    int getRegisterValue(const std::string& arg) const{
        return registers[getRegisterIndex(arg)];
    }

    void jnz(const std::string& arg1, const::std::string& arg2){
        int offset = (getRegisterValue(arg1) != 0) ? std::stoi(arg2) : static_cast<int>(1);
        instructionPointer += offset;
    }
    
    void sub(const std::string& arg1, const::std::string& arg2){
        registers[getRegisterIndex(arg1)] -= getRegisterOrValue(arg2);
    }
    
    void mul(const std::string& arg1, const::std::string& arg2){
        registers[getRegisterIndex(arg1)] *= getRegisterOrValue(arg2);
        numberOfMultiplicationsInvoked++;
    }

    size_t getRegisterIndex(const std::string& arg1) const {
        assert(arg1.length() == 1);
        return arg1[0] - 97;
    }

};

bool isComposite(unsigned int b) {
    unsigned int root = sqrt(b); // purposely truncate
    for(unsigned int factor = 2; factor < root+1 ; ++factor){
        if (b % factor == 0){
            return true;
        }
    }
    return false;
}

int main() {

    //part 2
    unsigned int numberOfTimesHIsHit = 0;
    for(unsigned int b = 109900; b <= 126900; b+=17){
        if(isComposite(b)){
            numberOfTimesHIsHit++;
        }
    }
    std::cout << " Number of h's hit: " << numberOfTimesHIsHit << "\n";

    // don't run the following unless you want to wait a very long time
    // auto instructions = algo::map(input::readMultiLineFile("input/input23.txt"), toInstruction);
    // Computer computer{instructions};
    // computer.run();
    // std::cout << "Multiplications hit: " << computer.getMultiplicationsInvoked() << "\n";
    return 0;
}