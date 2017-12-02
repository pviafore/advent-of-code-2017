#include "input.h"

#include <fstream>
#include <iterator>
#include <sstream>

namespace input {
    std::string readSingleLineFile(std::string fileName) {
        std::ifstream inFile;
        inFile.open(fileName);

        if(inFile.eof()) {
            throw std::runtime_error("The file should not have been empty)");
        }

        std::string input;
        getline(inFile, input);
        return input;
    }

    std::vector<std::string> readMultiLineFile(std::string fileName) {
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
}