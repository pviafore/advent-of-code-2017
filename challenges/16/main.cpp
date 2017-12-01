#include <algorithm>
#include <iostream>
#include <numeric>
#include <variant>
#include <vector>
#include <unordered_map>

#include "input.h"

struct Spin {
    unsigned long int times;
};
struct Exchange {
    unsigned long int pos1;
    unsigned long int pos2;
};
struct Partner {
    char program1;
    char program2;
};

using Step = std::variant<Spin,Exchange,Partner>;
using Steps = std::vector<Step>;

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

Exchange make_exchange(const std::string & text) {
    const auto positions = input::split(std::string(text.begin()+1, text.end()), '/');
    return Exchange{std::stoul(positions[0]),std::stoul(positions[1])};
}

Steps get_dance_steps(const std::string & filename) {
    const auto text = input::readSingleLineFile(filename);
    const auto textSteps = input::split(text, ',');
    Steps outVector;
    std::transform(textSteps.begin(), textSteps.end(), std::back_inserter(outVector), [](const auto& s) -> Step {
        switch (s[0]) {
            case 's':
                return Spin{std::stoul(std::string(s.begin()+1, s.end()))};
            case 'x':
                return make_exchange(s);
            case 'p':
                return Partner{s[1], s[3]};
            default:
                throw std::runtime_error("Invalid text step");
        }
    });
    return outVector;
}

void dance_step(std::string& programs, const Step& step){
    std::visit(overloaded {
        [&programs](const Spin& spin){ std::rotate(programs.rbegin(), programs.rbegin()+spin.times, programs.rend()); },
        [&programs](const Exchange& exchange){ std::swap(programs[exchange.pos1], programs[exchange.pos2]); },
        [&programs](const Partner& partner){
            auto pos1 = programs.find(partner.program1);
            auto pos2 = programs.find(partner.program2);
            std::swap(programs[pos1], programs[pos2]);
        }
    }, step);
}

std::string dance(std::string programs, const Steps& steps) {
    for(const Step& step: steps){
        //mutates programs
        dance_step(programs, step);
    }
    return programs;
}

std::string lots_of_dancing(std::string programs, const Steps& steps, unsigned int times){
    unsigned int counter = 0;
    std::unordered_map<std::string, unsigned int> seen = {{programs, 0}};
    while (counter < times) {
        programs = dance(programs, steps);
        if (auto iter = seen.find(programs); iter != seen.end()){
            auto difference = counter - iter->second;
            while (counter + difference < times) {
                counter += difference+1;
            }
        }
        seen.emplace(programs, ++counter);
    }
    return programs;
}



std::string programs = "abcdefghijklmnop";
int main() {
   const Steps steps = get_dance_steps("input/input16.txt");
   std::cout << "Order after dance: " << dance(programs, steps) << "\n";
   std::cout << "Order after lots of dance: " << lots_of_dancing(programs, steps, 1'000'000) << "\n";
}
