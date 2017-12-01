#include <array>
#include <ostream>
#include <sstream>
#include <vector>


namespace containers {


    template<typename T, size_t Size>
    class CircularBuffer {
        
    public:

        void append(T value) {
            if(hasWrappedAround) {
                if(current == begin) {
                    begin++;
                    if(begin == Size) {
                        begin = 0u;
                    }
                }
            }
            a[current++] = value;
            if(current == Size) {
                current = 0u;
                hasWrappedAround = true;
            }
        }

        std::vector<T> range(unsigned int b, unsigned int e) {
            std::vector<T> out;
            for(auto i = b; i < e; ++i) {
                out.push_back(a[(begin+i) % Size]);
            }
            return out;
        }

        T& operator[](unsigned int index) { 
            return a[index % Size];
        }

        std::string to_string() const {
            std::string out = "";
            for(auto i = 0u; i < Size; ++i) {
                out += std::to_string(a[(begin+i) % Size]) + " ";
            }
            return out;
        }


    private:
        bool hasWrappedAround = false;
        std::array<T, Size> a;
        unsigned int begin = 0u;
        unsigned int current = 0u;
    };


    template<typename T, size_t Size>
    std::ostream& operator<<(std::ostream& os, const CircularBuffer<T,Size>& cb) {
        os << cb.to_string() << "\n";
        return os;
    }
}

