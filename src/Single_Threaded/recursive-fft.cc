#include "../complex.h"
#include "../input_image.h"

#include <cmath>

const float PI = 3.14159265358979f;

void separate(Complex * array, size_t size) {
    // All evens to lower half; odds to upper half
    Complex *temp = new Complex[size];

    for (size_t i = 0; i < (size >> 1); ++i) {
        temp[i] = array[(i << 1) + 1];
    }
    for (size_t i = 0; i < (size >> 1); ++i) {
        array[i] = array[i << 1];
    }
    for (size_t i = 0; i < (size >> 1); ++i) {
        array[i + (size >> 1)] = temp[i];
    }

    delete[] temp;
}

void fft(Complex *input, int size) {
    if (size <= 1) {
        return;
    }

    //divide
    separate(input, size);

    //recursive call
    fft(input, size >> 1);
    fft(input + (size >> 1), size >> 1);

    //combine
    for (size_t i = 0; i < size / 2; ++i) {
        float theta = -2 * PI * i / size;
	    Complex even = input[i];
	    Complex odd = input[i + (size >> 1)];
        
	    Complex twiddle_factor = Complex(cos(theta), sin(theta)) * odd;
        input[i] = even + twiddle_factor;
        input[i + (size >> 1)] = even - twiddle_factor;
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

    // Transpose
    for (int row = 0; row < img_height; ++row) {
        for (int col = 0; col < img_width; ++col) {
            img_transpose[row + (col * img_height)] = img[col + (row * img_width)];
        }
    }

    for (int row = 0; row < img_width; ++row) {
        fft(img_transpose + row * img_height, img_height);
    }

    image_handler.save_image_data(argv[3], img_transpose, img_width, img_height);

    delete [] img_transpose;

    return 0;
}
