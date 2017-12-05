#include "input.h"

#include <iterator>
#include <sstream>

namespace input {
    std::string readSingleLineFile(const std::string& fileName) {
        std::ifstream inFile;
        inFile.open(fileName);

        if(inFile.eof()) {
            throw std::runtime_error("The file should not have been empty)");
        }

        std::string input;
        getline(inFile, input);
        return input;
    }

    std::vector<std::string> readMultiLineFile(const std::string& fileName) {
        std::ifstream inFile;
        inFile.open(fileName);

        std::vector<std::string> v;
        while(!inFile.eof()) {
            std::string input;
            getline(inFile, input);
            v.push_back(input);
        }
        
        return v;
    }

    std::vector<std::string> split(const std::string& str){
        std::istringstream iss(str);
        std::vector<std::string> v((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
        return v;
    }

    std::vector<int> toNumbers(const std::string &str) {
        auto v = split(str);
        std::vector<int> out;
        std::transform(v.begin(), v.end(), std::back_inserter(out), toNumber );
        return out;
    }

    int toNumber(std::string str) {
        return std::stoi(str);
    }
}