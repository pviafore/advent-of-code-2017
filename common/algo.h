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

    auto find_pairs(auto container, auto filterOperation) {
        using container_type = typename decltype(container.begin())::value_type; 
        std::vector<std::pair<container_type, container_type>> out;
        for (auto iter1 = container.begin(); iter1 != container.end(); ++iter1) {
            for(auto iter2 = iter1+1; iter2 != container.end(); ++iter2) {
                if (filterOperation(*iter1, *iter2)){
                    out.emplace_back(*iter1, *iter2);
                } 
                if (filterOperation(*iter2, *iter1)) {
                    out.emplace_back(*iter2, *iter1);
                }
            }
        }
        return out;
    }
}