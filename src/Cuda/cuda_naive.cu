//
// Created by jbm on 12/5/18.
//

#include <driver_types.h>
#include "cuda_naive.h"
#include <stdio.h>

__global__ void transpose(Complex * A, Complex * A_transpose, int w, int h) {
	int i = threadIdx.y + blockDim.y * blockIdx.y;
	int j = threadIdx.x + blockDim.x * blockIdx.x;
	if(i < h && j < w)
		A_transpose[j * w + i] = A[i * w + j];

}

__global__ void fft_row(Complex * input, Complex * output, int w, int h) {
	int i = threadIdx.y + blockDim.y * blockIdx.y;
	int j = threadIdx.x + blockDim.x * blockIdx.x;
	if(i < h && j < w) {
//		printf("hellow from (%d,%d)\n", i, j);
		output[i * w + j].real = 0;
		output[i * w + j].imag = 0;

		for (int k = 0; k < w; k++) {
			float W_real = cos(2 * M_PI * j * k / w);
//			printf("W_real: %f\n", W_real);
			float W_imag = -sin(2 * M_PI * j * k / w);
			output[i * w + j].real += (W_real * input[i * w + k].real - W_imag * input[i * w + k].imag);
			output[i * w + j].imag += (W_real * input[i * w + k].imag + W_imag * input[i * w + k].real);
		}
//		printf("r: %f\n",output[i * w + j].real);

	}


}


void cuda_naive(Complex * input_image, Complex * output_transform, InputImage input_image_meta) {
	Complex * d_input_image = NULL;
	Complex * d_output_transform = NULL;
	Complex * d_input_image_transpose = NULL;

	int w = input_image_meta.get_width();
	int h = input_image_meta.get_height();

	cudaMalloc((void**)&d_input_image, sizeof(Complex)*w*h);
	cudaMalloc((void**)&d_output_transform, sizeof(Complex)*w*h);
	cudaMalloc((void**)&d_input_image_transpose, sizeof(Complex)*w*h);

	cudaMemcpy(d_input_image, input_image, sizeof(Complex)*w*h, cudaMemcpyHostToDevice);

	int grid_height = ceil((float)h/THREADS_PER_BLOCK_SIDE);
	int grid_width = ceil((float)h/THREADS_PER_BLOCK_SIDE);

	fft_row<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_input_image, d_output_transform, w, h);
	transpose<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_output_transform, d_input_image_transpose, w, h);

	fft_row<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_input_image_transpose, d_output_transform, w, h);

	transpose<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_output_transform, d_input_image_transpose, w, h);

	cudaMemcpy(output_transform, d_input_image_transpose, sizeof(Complex)*w*h,cudaMemcpyDeviceToHost);
}
