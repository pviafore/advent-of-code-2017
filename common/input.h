#include <string>
#include <vector>

namespace input {
    //will throw an exception if the file is not found or if the file is empty
    std::string readSingleLineFile(std::string fileName);
    std::vector<std::string> readMultiLineFile(std::string fileName);
    
    std::vector<std::string> split(const std::string& str);
}