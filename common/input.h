#include <algorithm>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

namespace input {
    //will throw an exception if the file is not found or if the file is empty
    std::string readSingleLineFile(const std::string& fileName);    
    std::vector<std::string> split(const std::string& str);

    std::vector<std::string> readMultiLineFile(const std::string& fileName);

    auto readMultiLineFile(const std::string& fileName, auto rowOperation) {
        using ReturnType = typename std::invoke_result<decltype(rowOperation), std::string>::type;
        std::vector<ReturnType> v;
        auto strings = readMultiLineFile(fileName);
        std::transform(strings.begin(), strings.end(), std::back_inserter(v), rowOperation);
        return v;
    }

    std::vector<int> toNumbers(const std::string & str);


}