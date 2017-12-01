#include <string>

namespace input {
    //will throw an exception if the file is not found or if the file is empty
    std::string readSingleLineFile(std::string fileName);
}