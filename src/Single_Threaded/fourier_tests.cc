#include "../complex.h"
#include "../input_image.h"
#include "../fourier_transforms.h"

#include <cmath>
#include <iostream>

const float PI = 3.14159265358979f;

int main(int argc, char **argv) {

    Complex test_in[] = {Complex(1), Complex(2), Complex(1), Complex(1), Complex(1), Complex(1), Complex(1), Complex(2)};
    Complex *test_out = new Complex[8];
    dft(test_in, test_out, 8);
    for (int i = 0; i < 8; ++i) {
        std::cout << test_out[i] << std::endl;
    }
    
    std::cout << "//////////////////////////\n";

    Complex test_in1[] = {Complex(1), Complex(2), Complex(1), Complex(1), Complex(1), Complex(1), Complex(1), Complex(2)};
    inplace_fft(test_in1, 8);
    for (int i = 0; i < 8; ++i) {
        std::cout << test_in1[i] << std::endl;
    }

    std::cout << "//////////////////////////\n";

    Complex test_in2[] = {Complex(1), Complex(2), Complex(1), Complex(1), Complex(1), Complex(1), Complex(1), Complex(2)};
    recursive_fft(test_in2, 8);
    for (int i = 0; i < 8; ++i) {
        std::cout << test_in2[i] << std::endl;
    }

    return 0;
}
