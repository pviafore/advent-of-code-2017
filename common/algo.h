#include <vector>

namespace algo {
    auto zip(auto container1, auto container2) {
        auto iter1 = container1.begin();
        auto iter2 = container2.begin();

        using TypePair = std::pair<typename decltype(iter1)::value_type, typename decltype(iter2)::value_type>;
        std::vector<TypePair> out;
        while(iter1 != container1.end() && iter2 != container2.end()) {
            out.emplace_back(*iter1++, *iter2++);
        }
        return out;
    }  
}