#pragma once

#include "../complex.h"
#include "../input_image.h"
#include "../fourier_transforms.h"

#include <cmath>
#include <mpi.h>

void compose_complex_array(Complex *output, float *real, float *imag, int size);
void decompose_complex_array(Complex *input, float *out_real, float *out_imag, int size);
void distribute_mpi_data(int MPI_rank, int MPI_num_ranks, int chunk_size, int img_width, int img_height, Complex *img, Complex *elements);
void collect_mpi_data(int MPI_rank, int MPI_num_ranks, int chunk_size, int img_width, int img_height, Complex *img, Complex *elements);
void mpi_fft_2d(int argc, char **argv, bool is_reverse);
