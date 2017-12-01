#ifndef ALGO_H_
#define ALGO_H_

#include <algorithm>
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

    auto find_matching_pairs(auto container, auto filterOperation) {
        using ContainerType = typename decltype(container.begin())::value_type;
        std::vector<std::pair<ContainerType, ContainerType>> out;
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

    auto map(auto container, auto rowOperation) {
        using ContainerType = typename decltype(container.begin())::value_type;
        using ReturnType = typename std::invoke_result_t<decltype(rowOperation), ContainerType>;
        std::vector<ReturnType> v;
        std::transform(container.begin(), container.end(), std::back_inserter(v), rowOperation);
        return v;
    }

    auto range(auto start, auto end) {
        using RangeType = decltype(start);
        static_assert(std::is_same<RangeType, decltype(end)>::value, "Start and end must be the same type for range functions");
        std::vector<RangeType> v;
        for(RangeType r = start; r < end; ++r) {
            v.push_back(r);
        }
        return v;
    }

    auto apply_cyclically(auto &container, auto current, size_t numberOfTimes, auto operation) {
        while(numberOfTimes-- != 0) {
            if(current == container.end()) {
                current = container.begin();
            }

            operation(*current);
            current++;
        }
    }

    auto erase_if(auto& container, const auto & predicate) {
        auto it = container.begin();
        while(it!=container.end()) {
            if(predicate(*it)) {
                it = container.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    auto getNumericRange(auto start, auto end) {
        std::vector<int> r;
        for(int i = start; i < end; ++i ){
            r.push_back(i);
        }
        return r;
    }
}

#endif
