#include "../complex.h"
#include "../input_image.h"
#include "../fourier_transforms.h"

#include <cmath>

const float PI = 3.14159265358979f;

/*
void dft(Complex *input, Complex *output, int size) {
    for(int n = 0; n < size; ++n) {
        output[n] = Complex(0,0);
        for(int k = 0; k < size; ++k) {
            float theta = 2 * PI * n * k / size;
            Complex W = Complex(cos(theta), -sin(theta));
            output[n] = output[n] + W * input[k];

        }
    }
}
*/

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
    Complex *out = new Complex[img_width * img_height];

    // Row Major
    for(int row = 0; row < img_height; row++){
        dft(img + row * img_width, out + row * img_width, img_width);
    }

    // Transpose
    for (int row = 0; row < img_height; ++row) {
        for (int col = 0; col < img_width; ++col) {
	        img_transpose[row + (col * img_height)] = out[col + (row * img_width)];
	    }
    }

    // "Column Major" row major after a transpose
    for (int row = 0; row < img_width; ++row) {
        dft(img_transpose + row * img_height, out + row * img_height, img_height);
    }
    
    image_handler.save_image_data(argv[3], out, img_width, img_height);
    delete [] img_transpose;
    delete [] out;
    
    return 0;
}
