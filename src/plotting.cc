#include <vector>
#include <fstream>
#include <matplot/matplot.h>

typedef double Scalar;

// Зададим константу пи, вычисленную во время компиляции
constexpr double pi() { return std::atan(1)*4; }

Scalar signal(const float t) {
    return std::sin(2 * pi() * t) + std::cos(9 * pi() * pi() * t * t);
}

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

int main() {
    auto time = utils::load_vector("Time");
    auto discrete_signal = utils::load_vector("Discrete Signal");
    auto digital_signal = utils::load_vector("Digital Signal");
    auto binary_coded_signal = utils::load_vector("Binary Coded Signal");
    auto phys_level_encoded = utils::load_vector("Phys-Level Encoded");
    auto phys_level_decoded = utils::load_vector("Phys-Level Decoded");
    auto digital_signal_decoded = utils::load_vector("Digital Signal Decoded");
    auto analog_signal_after_convert = utils::load_vector("Analog signal after digital convertation");

    auto x = matplot::linspace(0, 1, 1000);
    std::vector<Scalar> y;
    for (auto x_ : x) {
        y.push_back(signal(x_));
    }

    matplot::plot(x, y, time, discrete_signal, "-o");
    matplot::title("Source analog signal");
    matplot::xlabel("Time (s)");
    matplot::ylabel("f(t)");
    matplot::legend({"Source Signal", "Discrete Signal"});
    matplot::save("img/1.eps");
    matplot::cla();

    matplot::plot(time, digital_signal, "-o");
    matplot::title("Digital signal");
    matplot::xlabel("Time (s)");
    matplot::ylabel("Quantized Signal Level");
    matplot::save("img/2.eps");
    matplot::cla();

    auto t = matplot::linspace(0, 1, binary_coded_signal.size());
    matplot::plot(t, binary_coded_signal);
    matplot::ylim({-2, 2});
    matplot::title("Coded signal");
    matplot::xlabel("Time (s)");
    matplot::save("img/3.eps");
    matplot::cla();

    auto p = matplot::plot(t, phys_level_encoded);
    p->line_width(1.5);
    matplot::ylim({-2, 2});
    matplot::title("Phys Level signal");
    matplot::xlabel("Time (s)");
    matplot::save("img/4.eps");
    matplot::cla();

    auto p2 = matplot::plot(t, binary_coded_signal, t, phys_level_encoded, "--");
    p2[0]->line_width(1.5);
    p2[1]->color("black");
    matplot::ylim({-2, 2});
    matplot::title("Phys Level signal With Binary Coded Signal");
    matplot::xlabel("Time (s)");
    matplot::save("img/5.eps");
    matplot::cla();

    matplot::plot(x, y, time, analog_signal_after_convert, "-o");
    matplot::title("Source analog signal With Image");
    matplot::xlabel("Time (s)");
    matplot::ylabel("f(t)");
    matplot::legend({"Source Signal", "Image Signal"});
    matplot::save("img/6.eps");
    matplot::cla();
}
