#include <algorithm>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

namespace input {
    //will throw an exception if the file is not found or if the file is empty
    std::string readSingleLineFile(const std::string& fileName);    
    std::vector<std::string> readMultiLineFile(const std::string& fileName);
    
    std::vector<std::string> split(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::vector<int> toNumbers(const std::string & str);
    int toNumber(std::string s);


}