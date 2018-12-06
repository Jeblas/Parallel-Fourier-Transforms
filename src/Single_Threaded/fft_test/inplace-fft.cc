#include "../complex.h"
#include "../input_image.h"

#include <iostream>
#include <cmath>

const float PI = 3.14159265358979f;

void fft(Complex *input, int size) {

    int n;
    int k = size;
    float theta = PI / size;
    Complex Wm = Complex(cos(theta), -1 * sin(theta));
    Complex T;
    while (k > 1) {
        n = k;
	k >>= 1;
	Wm = Wm * Wm;
        T = Complex(1);
        for (int l = 0; l < k; ++l) {
	    for (int a = l; a < size; a += n) {
	        int b = a + k;
		Complex t = input[a] - input[b];
		input[a] = input[a] + input[b];
		input[b] = t * T;
	    }
	    T = T * Wm;
	}
    }
    // m = Log2(size)
    int m = 0;
    for (int i = 2; i <= size; i = i + i) {
        ++m;
    }
    m = log2(size);
    
    // Bit Reverse copy
    // TODO Can make into lookup tabel stack overflow : Most Efficient Algorithm for Bit Reversal
    for (uint32_t a = 0; a < size; ++a) {
	uint32_t b = a;
	b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
	b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
	b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
	b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
        b = ((b >> 16) | (b << 16)) >> (32 - m);
	if (b > a) {
	    Complex temp = input[b];
	    input[b] = input[a];
	    input[a] = temp;
	}

    }
/*  
    // do Log(N) iterations of butterfly updates
    for (int L = 2; L <= size; L = L + L) {
	for (int k = 0; k < (L/2); ++k) {
	    
	    float theta = -2 * k * PI / L;
	    Complex wm = Complex(cos(theta), sin(theta));

	    for (int j = 0; j < (size / L); ++j) {
		int index_base = (j * L) + k;

		Complex twiddle_factor =  input[index_base + (L / 2)] * wm; 		// kth * odd element
		input[index_base + (L / 2)] = input[index_base] - twiddle_factor;
		input[index_base] = input[index_base] + twiddle_factor;
	    }
	}

    }
*/
/*
    for (int L = 2; L <= size; L = L + L) {
        float theta = 2 * PI / L;
	Complex Wm = Complex(cos(theta), -1 * sin(theta));
	for (int k = 0; k < size; k += L) {
	    Complex W = Complex(1);
	    for (int j = 0; j < L /2; ++j) {
	        Complex twiddle_factor = W * input[k + j + L/2];
		Complex even = input[k + j];
		input[k + j] = even + twiddle_factor;
		input[k + j + L/2] = even - twiddle_factor;
		W = W * Wm;
	    }

	}
    }
*/

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
