#include <vector>
#include <fstream>
#include <matplot/matplot.h>

typedef double Scalar;

namespace utils {
    std::vector<Scalar> load_vector(const std::string &filename) {
        std::ifstream ifs(filename.c_str(), std::ios::in | std::ifstream::binary);
        if (ifs.fail())
            throw std::runtime_error("Error while opening file");

        std::vector<char> buffer;
        std::istreambuf_iterator<char> iter(ifs);
        std::istreambuf_iterator<char> end;
        std::copy(iter, end, std::back_inserter(buffer));
        std::vector<Scalar> vec(buffer.size() / sizeof(Scalar));
        std::copy(&buffer[0], &buffer[0] + buffer.size(), reinterpret_cast<char *>(&vec[0]));
        return vec;
    }
}

// TODO: доделать
int main() {

}
