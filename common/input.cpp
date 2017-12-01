#include "input.h"

#include <fstream>
#include <type_traits>

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
            if(input != ""){
                v.push_back(input);
            }
        }

        return v;
    }

    std::vector<std::string> split(const std::string& str){
        std::istringstream iss(str);
        std::vector<std::string> v((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
        return v;
    }

    std::vector<std::string> split(const std::string& str, char delimiter){
        std::istringstream iss(str);
        std::vector<std::string> v;
        std::string token;
        while(std::getline(iss, token, delimiter)) {
            v.push_back(token);
        }
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

    std::string dropTrailingComma(const std::string &s) {
        if (*(s.end() - 1) == ',') {
            return std::string(s.begin(), s.end() - 1);
        }
        return s;
    }

    std::string join(const std::vector<std::string>& v, std::string delimiter) {
        std::stringstream ss;
        std::copy(v.begin(), v.end()-1, std::ostream_iterator<std::string>(ss, delimiter.c_str()));
        ss << *(v.end() - 1); // skip delimiter
        return ss.str();
    }
}
