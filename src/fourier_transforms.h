#pragma once

#include "complex.h"
#include <cmath>

void dft(Complex *input, Complex *output, int size);
void separate(Complex * array, size_t size);
void inplace_fft(Complex *input, int size);
void recursive_fft(Complex *input, int size);