//
// Created by jbm on 12/5/18.
//


#include "cuda_naive.h"


__global__ void transpose(Complex * A, Complex * A_transpose, int w, int h) {
	int i = threadIdx.y + blockDim.y * blockIdx.y;
	int j = threadIdx.x + blockDim.x * blockIdx.x;
	if(i < h && j < w)
		A_transpose[j * w + i] = A[i * w + j];

}

__global__ void idft_row(Complex * input, Complex * output, int w, int h) {
	int i = threadIdx.y + blockDim.y * blockIdx.y;
	int j = threadIdx.x + blockDim.x * blockIdx.x;

	int this_element = i * w + j;


	if(i < h && j < w) {

		output[this_element].real = 0;
		output[this_element].imag = 0;

		for (int k = 0; k < w; k++) {
			double angle = 2 * M_PI * j * k / w;
			float W_real = cos(angle);

			float W_imag = sin(angle);
			int neighboring_element = i * w + k;
			output[this_element].real += (W_real * input[neighboring_element].real - W_imag * input[neighboring_element].imag);
			output[this_element].imag += (W_real * input[neighboring_element].imag + W_imag * input[neighboring_element].real);
		}
		output[this_element].real /= w;
		output[this_element].imag /= w;


	}
}

__global__ void dft_row(Complex * input, Complex * output, int w, int h) {
	int i = threadIdx.y + blockDim.y * blockIdx.y;
	int j = threadIdx.x + blockDim.x * blockIdx.x;

	int this_element = i * w + j;


	if(i < h && j < w) {

		output[this_element].real = 0;
		output[this_element].imag = 0;

		for (int k = 0; k < w; k++) {
			double angle = 2 * M_PI * j * k / w;
			float W_real = cos(angle);

			float W_imag = -sin(angle);
			int neighboring_element = i * w + k;
			output[this_element].real += (W_real * input[neighboring_element].real - W_imag * input[neighboring_element].imag);
			output[this_element].imag += (W_real * input[neighboring_element].imag + W_imag * input[neighboring_element].real);
		}


	}


}


void cuda_naive_forward(Complex * input_image, Complex * output_transform, InputImage input_image_meta) {
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

	dft_row<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_input_image, d_output_transform, w, h);
	transpose<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_output_transform, d_input_image_transpose, w, h);

	dft_row<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_input_image_transpose, d_output_transform, w, h);

	transpose<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_output_transform, d_input_image_transpose, w, h);

	cudaMemcpy(output_transform, d_input_image_transpose, sizeof(Complex)*w*h,cudaMemcpyDeviceToHost);
}

void cuda_naive_inverse(Complex * input_image, Complex * output_transform, InputImage input_image_meta) {

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

	idft_row<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_input_image, d_output_transform, w, h);
	transpose<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_output_transform, d_input_image_transpose, w, h);

	idft_row<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_input_image_transpose, d_output_transform, w, h);

	transpose<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_output_transform, d_input_image_transpose, w, h);

	cudaMemcpy(output_transform, d_input_image_transpose, sizeof(Complex)*w*h,cudaMemcpyDeviceToHost);
}