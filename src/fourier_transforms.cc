#include "fourier_transforms.h"

const float PI = 3.14159265358979f;

///////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////

void separate(Complex * array, size_t size) {
    // All evens to lower half; odds to upper half
    Complex *temp = new Complex[size / 2];

    for (size_t i = 0; i < (size / 2); ++i) {
        temp[i] = array[(i * 2) + 1];
    }
    for (size_t i = 0; i < (size / 2); ++i) {
        array[i] = array[i * 2];
    }
    for (size_t i = 0; i < (size / 2); ++i) {
        array[i + (size / 2)] = temp[i];
    }

    delete[] temp;
}

void inplace_fft(Complex *input, int size) {
    if (size <= 1) {
        return;
    }

    //divide
    separate(input, size);

    //recursive call
    inplace_fft(input, size / 2);
    inplace_fft(input + (size / 2), size / 2);

    //combine
    for (int i = 0; i < (size / 2); ++i) {
        float theta = -2.0f* PI * i / size;
	Complex even = input[i];
	Complex odd = input[i + (size / 2)];
        
	Complex twiddle_factor = Complex(cos(theta), sin(theta)) * odd;
        input[i] = even + twiddle_factor;
        input[i + (size / 2)] = even - twiddle_factor;
    }
}

///////////////////////////////////////////////////////////////

void recursive_fft(Complex *input, int size) {
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
    recursive_fft(even, size >> 1);
    recursive_fft(odd, size >> 1);

    //combine
    for (size_t i = 0; i < size / 2; ++i) {
        float theta = -2 * PI * i / size;
        Complex temp = Complex(cos(theta), sin(theta)) * odd[i];
        input[i] = even[i] + temp;
        input[i + (size >> 1)] = even[i] - temp;
    }
}