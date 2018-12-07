//
// Created by jbm on 12/5/18.
//

#ifndef P3_CUDA_NAIVE_H
#define P3_CUDA_NAIVE_H
#include "../complex.h"
#include "../input_image.h"
#include <cmath>
#include <stdio.h>
#include <iomanip>
#include <cuComplex.h>
#include <cusparse.h>
#include <inttypes.h>
#define THREADS_PER_BLOCK_SIDE 8


struct csr_complex_Mat {
	int nnz;
	cuComplex * csrValA;
	int * csrRowPtrA;
	int * csrColIndA;
};

void cuda_naive(Complex * input_image, Complex * output_transform, InputImage input_image_meta, FourierDirection dir);

void cuda_ufft(Complex * input_image, Complex * output_transform, InputImage inputImage);

#endif //P3_CUDA_NAIVE_H
