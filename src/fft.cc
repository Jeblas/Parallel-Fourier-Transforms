#include "complex.h"
#include "input_image.h"

#include <iostream>
#include <cmath>

const float PI = 3.14159265358979f;

void fft(Complex *input, int size) {
    if (size <= 1) {
        return;
    }

    //divide
    Complex even[size >> 1];
    Complex odd[size >> 1];
    for (size_t k = 0; k < (size >> 1); ++k) {
	even[k] = input[k << 1];
	odd[k] = input[(k << 1) + 1];
    }

    //recursive call
    fft(even, size >> 1);
    fft(odd, size >> 1);

    //combine
    for (size_t i = 0; i < size / 2; ++i) {
        float theta = -2 * PI * i / size;
        Complex temp = Complex(cos(theta), sin(theta)) * odd[i];
        input[i] = even[i] + temp;
        input[i + (size >> 1)] = even[i] - temp;
    }
}

int main(int argc, char **argv) {
    Complex in_img [8];
    Complex *out_img;
    int img_width;
    int img_height;

    //InputImage image_handler(argv[2]);

    //in_img = image_handler.get_image_data();
    //img_width = image_handler.get_width();
    //img_height = image_handler.get_height();

    // Data element read in.

    in_img[0] = Complex(1.0);
    in_img[1] = Complex(1.0);
    in_img[2] = Complex(1.0);
    in_img[3] = Complex(1.0);
    in_img[4] = Complex(0.0);
    in_img[5] = Complex(0.0);
    in_img[6] = Complex(0.0);
    in_img[7] = Complex(0.0);

    fft(in_img, 8);
    for (int i = 0; i < 8; ++i) {
        std::cout << in_img[i] << std::endl;
    }

    //image_handler.save_image_data(argv[3], out_img, img_width, img_height);
    return 0;
}
