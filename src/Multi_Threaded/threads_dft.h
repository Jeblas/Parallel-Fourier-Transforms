#pragma once

#include "../complex.h"
#include "../input_image.h"
#include "../fourier_transforms.h"

#include <cmath>
#include <thread>
#include <vector>

void thread_dft_loop(Complex *input, Complex *output, int chunk_size, int thread_id, int num_cols, bool is_reverse);
void launch_threads_dft(std::vector<std::thread> & threads, Complex *input, Complex *output, int num_rows, int num_cols, bool is_reverse);
void mt_dft_2d(int argc, char **argv, bool is_reverse);
