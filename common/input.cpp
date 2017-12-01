#include "input.h"

#include <fstream>

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
}