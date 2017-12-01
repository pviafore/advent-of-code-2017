#include <array>
#include <iostream>
#include <optional>
#include <queue>
#include <variant>

#include "algo.h"
#include "input.h"

struct UnaryInstruction {
    char registerName;
};

struct BinaryInstruction {
    char registerName;
    int  value;
    bool is_register;
};

struct Snd : UnaryInstruction {};
struct Set : BinaryInstruction {};
struct Add : BinaryInstruction {};
struct Mul : BinaryInstruction {};
struct Mod : BinaryInstruction {};
struct Jgz : BinaryInstruction {};
struct Rcv : UnaryInstruction {};


// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

using Instruction = std::variant<Snd, Set, Add, Mul, Mod, Jgz, Rcv>;


class Computer{
public:
    Computer(const std::vector<Instruction>& instructions) : instructions(instructions) {
        std::fill(registers.begin(), registers.end(), 0);
        instructionPointer = 0;
    }

    bool isHalted() const {
        return halted;
    }


protected:

    void runSingleInstruction() {
        const auto& instruction = instructions[instructionPointer];
        std::visit( overloaded {
            [this](Snd snd){this->snd(snd);},
            [this](Set set){this->set(set);},
            [this](Add add){this->add(add);},
            [this](Mul mul){this->mul(mul);},
            [this](Mod mod){this->mod(mod);},
            [this](Rcv rcv){this->rcv(rcv);},
            [this](Jgz jgv){this->jgv(jgv);},
        }, instruction);
    }
    std::array<long, 26> registers;
    std::vector<Instruction> instructions;
    unsigned int instructionPointer = 0;
    bool halted = false;

private:
    virtual void snd(const Snd& snd) = 0;
    virtual void rcv(const Rcv& rcv) = 0;

    int get_value(int value, bool is_register){
        return is_register ? registers[value] : value;
    }

    void set(const Set& set){
        registers[set.registerName] = get_value(set.value, set.is_register);
        ++instructionPointer;
    }

    void add(const Add& add){
        registers[add.registerName] += get_value(add.value, add.is_register);
        ++instructionPointer;
    }

    void mul(const Mul& mul){
        registers[mul.registerName] *= get_value(mul.value, mul.is_register);
        ++instructionPointer;
    }

    void mod(const Mod& mod){
        registers[mod.registerName] %= get_value(mod.value, mod.is_register);
        ++instructionPointer;
    }

    void jgv(const Jgz& jgv){
        instructionPointer += (registers[jgv.registerName] > 0 ? get_value(jgv.value, jgv.is_register) : 1 );
        if(instructionPointer >= instructions.size()){
            halted = true;
        }
    }


};


Instruction to_instruction(const std::string& str) {
    std::string instruction = std::string(str.begin(), str.begin()+3);
    char registerName = str[4] - 97;
    int value = 0;
    bool is_register = false;
    if (str.size() > 5) {
        try{
            value = std::stoi(std::string(str.begin()+6, str.end()));
        }
        catch(std::invalid_argument&){
            value = str[6] - 97;
            is_register=true;
        }
    }
    if (instruction == "snd"){
        return Snd{registerName};
    }
    if (instruction == "set"){
        return Set{registerName, value, is_register};
    }
    if (instruction == "add"){
        return Add{registerName, value, is_register};
    }
    if (instruction == "mul"){
        return Mul{registerName, value, is_register};
    }
    if (instruction == "mod"){
        return Mod{registerName, value, is_register};
    }
    if (instruction == "rcv"){
        return Rcv{registerName};
    }
    if (instruction == "jgz"){
        return Jgz{registerName, value, is_register};
    }
    throw std::runtime_error("Invalid instruction");
}

class SoundComputer : public Computer {

public:

    SoundComputer(const std::vector<Instruction>& instructions) : Computer(instructions) {}

    void runUntilRcv(){
        while(receivesHit == 0){
            runSingleInstruction();
        }
    }

    int getLastRecoveredSound(){
        return lastSound;
    }

private:
    void snd(const Snd& snd) override {
        lastSound = registers[snd.registerName];
        ++instructionPointer;
    }

    void rcv(const Rcv& rcv) override {
        if (registers[rcv.registerName] > 0) {
            receivesHit++;
        }
        instructionPointer++;
    }

    unsigned int receivesHit = 0;
    unsigned int lastSound = 0;
};

class MessagingComputer : public Computer {
public:
    MessagingComputer(int id, const std::vector<Instruction>& instructions) : Computer(instructions) {
        registers[15] = id;
    }

    friend void pair(MessagingComputer& lhs, MessagingComputer& rhs);

    unsigned int get_number_of_values_sent() const {
        return numberOfValuesSent;
    }

    using Computer::runSingleInstruction;

private:

    void snd(const Snd& snd) {
        if(partner) {
            partner->messages.push(registers[snd.registerName]);
            partner->waiting = false;
            ++numberOfValuesSent;
        }
        ++instructionPointer;
    }

    void rcv(const Rcv& rcv) {
        if(messages.empty()){
            waiting = true;
            if (partner && partner->waiting){
                halted = true;
                partner->halted = true;
            }
        }
        else {
            registers[rcv.registerName] = messages.front();
            messages.pop();
            ++instructionPointer;
        }
    }
    std::queue<int> messages;
    MessagingComputer* partner = {};
    unsigned int numberOfValuesSent = 0;
    bool waiting = false;

};

void pair(MessagingComputer& lhs, MessagingComputer& rhs){
    lhs.partner = &rhs;
    rhs.partner = &lhs;
}

int main() {
    const auto instructions = algo::map(input::readMultiLineFile("input/input18.txt"), to_instruction);
    SoundComputer computer(instructions);
    computer.runUntilRcv();
    std::cout << computer.getLastRecoveredSound() << "\n";

    MessagingComputer computer0(0,instructions);
    MessagingComputer computer1(1,instructions);
    pair(computer0, computer1);

    while(!computer0.isHalted() && !computer1.isHalted() ) {
        computer0.runSingleInstruction();
        computer1.runSingleInstruction();
    }

    std::cout << "Times sent: " << computer1.get_number_of_values_sent() << "\n";
}
