#include "fourier_transforms.h"

const float PI = 3.14159265358979f;

///////////////////////////////////////////////////////////////

void dft(Complex *input, Complex *output, int size) {
    for(int n = 0; n < size; ++n) {
        output[n] = Complex(0,0);
        for(int k = 0; k < size; ++k) {
            float theta = -2.0f * PI * n * k / size;
            Complex W = Complex(cos(theta), sin(theta));
            output[n] = output[n] + (input[k] * W);
        }
    }
}

///////////////////////////////////////////////////////////////

void inverse_dft(Complex *input, Complex *output, int size) {
    for(int n = 0; n < size; ++n) {
        output[n] = Complex(0,0);
        for(int k = 0; k < size; ++k) {
            float theta = 2.0f * PI * n * k / size;
            Complex W = Complex(cos(theta), sin(theta));
            output[n] = output[n] + (input[k] * W);
        }
        output[n].real = output[n].real/size;
        output[n].imag = output[n].imag/size;
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

///////////////////////////////////////////////////////////////

void inverse_recursive_fft(Complex *input, int size) {

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
    inverse_recursive_fft(even, size >> 1);
    inverse_recursive_fft(odd, size >> 1);

    //combine
    for (size_t i = 0; i < size / 2; ++i) {
        float theta = 2 * PI * i / size;
        Complex temp = Complex(cos(theta), sin(theta)) * odd[i];
        input[i] = even[i] + temp;
        input[i + (size >> 1)] = even[i] - temp;
    }
    // need to normalize outside of function
}

///////////////////////////////////////////////////////////////

void inverse_inplace_fft(Complex *input, int size) {
    if (size <= 1) {
        return;
    }

    //divide
    separate(input, size);

    //recursive call
    inverse_inplace_fft(input, size / 2);
    inverse_inplace_fft(input + (size / 2), size / 2);

    //combine
    for (int i = 0; i < (size / 2); ++i) {
        float theta = 2.0f* PI * i / size;
	Complex even = input[i];
	Complex odd = input[i + (size / 2)];
        
	Complex twiddle_factor = Complex(cos(theta), sin(theta)) * odd;
        input[i] = even + twiddle_factor;
        input[i + (size / 2)] = even - twiddle_factor;
    }
    // need to normalize outside of function
}

///////////////////////////////////////////////////////////////

void non_recursive_inplave_fft(Complex *input, int size) {
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
}
