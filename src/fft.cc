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
    Complex *img;
    Complex *img_transpose;
    int img_width;
    int img_height;

    InputImage image_handler(argv[2]);

    img = image_handler.get_image_data();
    img_width = image_handler.get_width();
    img_height = image_handler.get_height();

    img_transpose = new Complex[img_width * img_height];

    for (int row = 0; row < img_height; ++row) {
        fft(img + row * img_width, img_width);
    }

    // transpose
    for (int row = 0; row < img_height; ++row) {
        for (int col = 0; col < img_width; ++col) {
            img_transpose[row + (col * img_height)] = img[col + (row * img_width)];
        }
    }

    for (int row = 0; row < img_width; ++row) {
        fft(img_transpose + row * img_height, img_height);
    }

    image_handler.save_image_data(argv[3], img_transpose, img_width, img_height);

    delete[] img_transpose;

    return 0;
}
