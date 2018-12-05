#include "complex.h"
#include "input_image.h"

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
    for (size_t i = 0; i < size / 2; ++i) {
        //
        // Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k]
            // odd[k] = in[(start + step) + (step * 2 * i)]
            // std::polar(1.0, -2 * PI * i / size)      =  r * thetha 
                // Complex(cos(-2 * PI * i / size), sin(-2 * PI * i / size))


            //auto value = funct (number/ (size))  * in[(start + step) + (step * 2 * i)] // odd [k]
        
        // in[k] = even[k] + t
            // in[k]            = in[ start + step * i ]
            // even[k] =    in[start + (step * 2 * i)]

        // in[k + N/2] = even[k] - t
        
            // in [k + N/2]     = in [(start + step * i) + (step * size / 2 )]
            // even[k] =    in[start + (step * 2 * i)]

        size_t even_ith_index = start + (step * 2 * i);
        float theta = -2 * PI * i / size;
        size_t index_start = start + step * i;

        Complex temp = Complex(cos(theta), sin(theta)) * in[(start + step) + (step * 2 * i)];
        in[index_start] = in[even_ith_index] + temp;
        in [index_start + (step * size / 2 )] = in[even_ith_index] - temp;
    }
}

int main(int argc, char **argv) {
    Complex *in_img;
    Complex *out_img;
    int img_width;
    int img_height;

    InputImage image_handler(argv[2]);

    in_img = image_handler.get_image_data();
    img_width = image_handler.get_width();
    img_height = image_handler.get_height();

    // Data element read in.
    fft(in_img, img_width, 1, 0);


    image_handler.save_image_data(argv[3], out_img, img_width, img_height);
    return 0;
}
