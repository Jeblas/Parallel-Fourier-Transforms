#include "../complex.h"
#include "../input_image.h"

#include <iostream>
#include <cmath>

const float PI = 3.14159265358979f;

void fft(Complex *in, int size, int step, int start) {
    if (size <= 1) {
        return;
    }

    //divide
    //recursive call
    fft(in, size / 2, step * 2, start);             // Even
    fft(in, size / 2, step * 2, start + step);      // Odd

    //combine
    //
    //
/*
    std::cout << "=====" << std::endl;
    std::cout << "size = " << size << std::endl;
    std::cout << "step = " << step << std::endl;
    std::cout << "start = " << size << std::endl;
    //Complex copy[size];

    for (size_t k = 0; k < size; ++k) {
        copy[k] = in[start + step * k];
	std::cout << start + step * k << '\n';
    }
    
*/
    Complex *temp = new Complex[size];
    for (size_t i = 0; i < size; ++i) {
        temp[i] = in[start + i * step];
    }

    for (size_t i = 0; i < size / 2; ++i) {
/*
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k]
            odd[k] = in[(start + step) + (step * 2 * i)]
            std::polar(1.0, -2 * PI * i / size)      =  r * thetha 
                Complex(cos(-2 * PI * i / size), sin(-2 * PI * i / size))
             auto value = funct (number/ (size))  * in[(start + step) + (step * 2 * i)] // odd [k]
        
        in[k] = even[k] + t
            in[k]            = in[ start + step * i ]
            even[k] =    in[start + (step * 2 * i)]

        in[k + N/2] = even[k] - t
            in [k + N/2]     = in [(start + step * i) + (step * size / 2 )]
            even[k] =    in[start + (step * 2 * i)]
*/
        Complex even = temp[i * 2];
	Complex odd = temp[i * 2 + 1];

	
        size_t even_ith_index = start + (step * 2 * i);
        float theta = -2 * PI * i / size;
        size_t index_start = start + step * i;

        //Complex even = in[even_ith_index];
	//Complex odd = in[(start + step) + (step * 2) * i];
        Complex twiddle_factor = Complex(cos(theta), sin(theta)) * odd;
        in[index_start] = even + twiddle_factor;
        in [index_start + (step * size / 2 )] = even - twiddle_factor;

/*	
	std::cout << i << " ------\n";
	std::cout << "in [" << index_start << "] = even [" << even_ith_index << "]\n";
	std::cout << "in [" << index_start + step * size / 2 << "] = even [" << even_ith_index << "]\n";
*/
/*
	Complex temp = Complex(cos(theta), sin(theta)) * in[(start + step) + (step * 2 * i)];
        in[index_start] = in[even_ith_index] + temp;
        in [index_start + (step * size / 2 )] = in[even_ith_index] - temp;
*/
    }
	delete[] temp;
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

    // Data element read in.
/*
    in_img[0] = Complex(1.0);
    in_img[1] = Complex(1.0);
    in_img[2] = Complex(1.0);
    in_img[3] = Complex(1.0);
    in_img[4] = Complex(0.0);
    in_img[5] = Complex(0.0);
    in_img[6] = Complex(0.0);
    in_img[7] = Complex(0.0);

    fft(in_img, 8, 1, 0);
    std::cout << "======================\n";
    for (int i = 0; i < 8; ++i) {
        std::cout << in_img[i] << std::endl;
    }
*/
    for (int row = 0; row < img_height; ++row) {
        fft(img + row * img_width, img_width, 1, 0);
    }

    // Transpose
    for (int row = 0; row < img_height; ++row) {
	for (int col = 0; col < img_width; ++col) {
	    img_transpose[row + (col * img_height)] = img[col + (row * img_width)];    
	}
    }

    for (int row = 0; row < img_width; ++row) {
	fft(img_transpose + row * img_height, img_height, 1, 0);
    }

    image_handler.save_image_data(argv[3], img_transpose, img_width, img_height);
    delete[] img_transpose;
    return 0;
}
