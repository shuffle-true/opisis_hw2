#include <vector>
#include <cmath>
#include <matplot/matplot.h>
#include <bitset>
#include <iterator>

typedef double Scalar;

namespace utils {
    void save_vector(const std::vector<Scalar> &vec, const std::string &filename) {
        std::ofstream ofs(filename.c_str(), std::ios::out | std::ios::binary);
        if (ofs.fail()) {
            throw std::runtime_error("Error when opening file");
        }

        std::ostream_iterator<char> osi(ofs);
        const char *begin_byte = reinterpret_cast<const char *>(&vec[0]);
        const char *end_byte = begin_byte + vec.size() * sizeof(Scalar);
        std::copy(begin_byte, end_byte, osi);
    }
}

// Зададим константу пи, вычисленную во время компиляции
constexpr double pi() { return std::atan(1)*4; }

Scalar signal(const float t) {
    return std::sin(2 * pi() * t) + std::cos(9 * pi() * pi() * t * t);
}

int main() {
    const size_t num_bits = 8;
    const size_t disc_freq = 50;
    const size_t time_frame = 1;

    /*
     * Оцифруем сигнал, для этого выполним 2 операции - дискретизация и квантование.
     */

    /*
     * Сначала выполним дискретизацию сигнала - разбиение по Ox
     */
    std::vector<Scalar> time_t;
    std::vector<Scalar> x_t;
    std::vector<Scalar> y_t;
    Scalar scale_ = 1 / (Scalar) disc_freq;

    for (size_t i = 0; i < disc_freq * time_frame; i++) {
        time_t.push_back(scale_ * i);
        x_t.push_back(signal(scale_ * i));
    }

    /*
     * Теперь квантование
     */

    Scalar a_min = *std::min_element(x_t.begin(), x_t.end());
    Scalar a_max = *std::max_element(x_t.begin(), x_t.end());
    Scalar scale_quant = (a_max - a_min) / ((2 << (num_bits - 1)) - 1);
    for (auto x : x_t) {
        y_t.push_back(std::round((x - a_min) / scale_quant));
    }

    /*
     * Теперь закодируем последовательность используя натуральный двоичный код
     */

    std::vector<Scalar> y_b;
    for (auto y : y_t) {
        std::bitset<num_bits> bitset(y);
        auto bitset_str = bitset.to_string();
        std::cout << y << " ";
        for (auto b : bitset_str) {
            y_b.push_back(strtod(&b, nullptr));
        }
    } std::cout << std::endl;

    /*
     * Закоидруем физический уровень используя бинарное кодирование
     */

    std::vector<Scalar> phys_encoded;
    Scalar prev = -1;
    for (auto yb : y_b) {
        if (yb == 0) {
            phys_encoded.push_back(0);
        } else {
            prev *= -1;
            phys_encoded.push_back(prev);
        }
    }


    /*
     * Расходируем физический уровень
     */

    std::vector<Scalar> phys_decoded;
    for (auto p : phys_encoded) {
        phys_decoded.push_back(std::abs(p));
    }

    /*
     * Раскодируем бинарный код
     */

    std::vector<Scalar> y_b_decoded;
    for (size_t i = 0; i < y_t.size(); i++) {
        Scalar binary_repr = 0;
        for (size_t j = i * num_bits; j < (i + 1) * num_bits; j++) {
            binary_repr += y_b[j] * (2 << (num_bits - j % num_bits - 2));
        }
        std::cout << binary_repr << " ";
        y_b_decoded.push_back(binary_repr);
    }

    /*
     * Конвертируем цифровой сигнал в аналоговый
     */

    std::vector<Scalar> x_t_analog;
    for (size_t i = 0; i < y_b_decoded.size(); i++) {
        x_t_analog.push_back(y_b_decoded[i] * (a_max - a_min) / (2 << (num_bits - 1)) + a_min);
    }

    utils::save_vector(time_t, "Time");
    utils::save_vector(x_t, "Discrete Signal");
    utils::save_vector(y_t, "Digital Signal");
    utils::save_vector(y_b, "Binary Coded Signal");
    utils::save_vector(phys_encoded, "Phys-Level Encoded");
    utils::save_vector(phys_decoded, "Phys-Level Decoded");
    utils::save_vector(y_b_decoded, "Digital Signal Decoded");
    utils::save_vector(x_t_analog, "Analog signal after digital convertation");
}
