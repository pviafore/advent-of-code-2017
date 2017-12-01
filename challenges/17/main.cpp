#include <iostream>
#include <vector>

constexpr size_t SPINS = 328;

class SpinlockAlgorithm{
public:
    SpinlockAlgorithm() : values{0}{}

    void insertNext() {
        lastPosition = ((SPINS + lastPosition) % values.size()) + 1;
        values.push_back(lastPosition);
        for(auto i=0u; i < values.size(); ++i){
            if (values[i] >= lastPosition && i != values.size()-1) {
                ++values[i];
            }
        }
    }

    size_t getNumberAfter(int val) {
        for (auto i=0u; i < values.size(); ++i){
            if(values[i] == values[val]+1){
                return i;
            }
        }
        throw std::runtime_error("Should not get this far");
    }

private:
    size_t lastPosition = 0;
    std::vector<unsigned int> values;
};

class AngrySpinlockAlgorithm {

public:
    void insertNext() {
        lastPosition = ((SPINS + lastPosition) % numbersInserted) + 1;

        if (lastPosition == zeroPosition+1){
            valueAfterZero = numbersInserted;
        }
        if (lastPosition <= zeroPosition) {
            ++zeroPosition;
        }
        ++numbersInserted;
    }

    unsigned int getNumberAfterZero() {
        return valueAfterZero;
    }


private:
    unsigned int zeroPosition = 0;
    unsigned int lastPosition = 0;
    unsigned int numbersInserted = 1;
    unsigned int valueAfterZero = 0;

};

size_t get_value_after_2017() {
    SpinlockAlgorithm spinlock;
    for(auto i=0; i<2017; ++i){
        spinlock.insertNext();
    }
    return spinlock.getNumberAfter(2017);
}

unsigned int get_angry_spinlock_value() {
    AngrySpinlockAlgorithm spinlock;
    for(auto i=0; i<50'000'000; ++i){
        spinlock.insertNext();
    }
    return spinlock.getNumberAfterZero();
}

int main() {
    std::cout << "Value after 2017: " <<  get_value_after_2017() << "\n";
    std::cout << "Value after 50,000,000: " <<  get_angry_spinlock_value() << "\n";
    return 0;
}
