//
// Created by jbm on 12/5/18.
//

#ifndef P3_CUDA_NAIVE_H
#define P3_CUDA_NAIVE_H
#include "../complex.h"
#include "../input_image.h"
#include <cmath>
#include <stdio.h>
#define THREADS_PER_BLOCK_SIDE 8

void cuda_naive_forward(Complex * input_image, Complex * output_transform, InputImage input_image_meta);
void cuda_naive_inverse(Complex * input_transform, Complex * ouput_image, InputImage input_image_mets);

#endif //P3_CUDA_NAIVE_H
