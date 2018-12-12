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
__global__ void transpose(cuComplex * A, cuComplex * A_transpose, int w, int h) {
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

void cuda_2d_dft(Complex * input_image, Complex * output_transform, InputImage input_image_meta, FourierDirection dir) {
	Complex * d_input_image = NULL;
	Complex * d_output_transform = NULL;
	Complex * d_input_image_transpose = NULL;
	std::cout << dir << std::endl;
	int w = input_image_meta.get_width();
	int h = input_image_meta.get_height();

	cudaMalloc((void**)&d_input_image, sizeof(Complex)*w*h);
	cudaMalloc((void**)&d_output_transform, sizeof(Complex)*w*h);
	cudaMalloc((void**)&d_input_image_transpose, sizeof(Complex)*w*h);

	cudaMemcpy(d_input_image, input_image, sizeof(Complex)*w*h, cudaMemcpyHostToDevice);

	int grid_height = ceil((float)h/THREADS_PER_BLOCK_SIDE);
	int grid_width = ceil((float)h/THREADS_PER_BLOCK_SIDE);

	if(dir == FORWARD) {
		dft_row << < dim3(grid_width, grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE) >> >
													(d_input_image, d_output_transform, w, h);
	} else if(dir == REVERSE) {
		idft_row << < dim3(grid_width, grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE) >> >
													 (d_input_image, d_output_transform, w, h);
	}

	transpose<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_output_transform, d_input_image_transpose, w, h);

	if(dir == FORWARD) {
		dft_row << < dim3(grid_width, grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE) >> >
													(d_input_image_transpose, d_output_transform, w, h);
	}else if(dir == REVERSE) {
		idft_row << < dim3(grid_width, grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE) >> >
													 (d_input_image_transpose, d_output_transform, w, h);
	}
	transpose<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_output_transform, d_input_image_transpose, w, h);

	cudaMemcpy(output_transform, d_input_image_transpose, sizeof(Complex)*w*h,cudaMemcpyDeviceToHost);
}

csr_complex_Mat generate_B_k_csr(int n) {
	csr_complex_Mat ret;
	ret.nnz = 2*n;
	ret.csrValA = (cuComplex *) malloc(sizeof(cuComplex)*ret.nnz);
	ret.csrColIndA = (int *) malloc(sizeof(int)*ret.nnz);
	ret.csrRowPtrA = (int *) malloc(sizeof(int)*(n + 1));
	for(int i = 0; i < ret.nnz; i+=2) {
		ret.csrValA[i] = make_cuComplex(1, 0);
	}
	for(int i = 1; i < ret.nnz/2; i+=2) {
		float arg = M_PI*(i-1)/n;
		ret.csrValA[i] = make_cuComplex(cos(arg),-sin(arg));
	}
	for(int i = ret.nnz/2 + 1; i < ret.nnz;i += 2) {
		float arg = M_PI*(i - 1 - ret.nnz/2)/n;
		ret.csrValA[i] = make_cuComplex(-cos(arg), sin(arg));
	}
	for(int i = 0; i < ret.nnz/2; i+=2) {
		ret.csrColIndA[i] = i/2;
		ret.csrColIndA[i + ret.nnz/2] = i/2;
	}
	for(int i = 1; i < ret.nnz/2; i+=2) {
		ret.csrColIndA[i] = i/2 + n/2;
		ret.csrColIndA[i + ret.nnz/2] = i/2 + n/2;
	}
	for(int i = 0; i < n; i ++) {
		ret.csrRowPtrA[i] = 2*i;
	}
	ret.csrRowPtrA[n] = ret.nnz;
	return ret;
}

csr_complex_Mat kronecker_I_B_k(int n, int k, csr_complex_Mat B_k) {
	csr_complex_Mat ret;

	int dim_I = n/k;
	ret.nnz = B_k.nnz*dim_I;
	ret.csrValA = (cuComplex *) malloc(sizeof(cuComplex)*ret.nnz);
	ret.csrColIndA = (int *) malloc(sizeof(int)*ret.nnz);
	ret.csrRowPtrA = (int *) malloc(sizeof(int)*(n + 1));
//	for(int i = 0; i < B_k.nnz; i++) {
//		for(int j =0; j < dim_I; j++){
//
//		}
//	}
	for(int i = 0; i < n; i++) {
		ret.csrRowPtrA[i] = 2*i;
	}
	ret.csrRowPtrA[n] = ret.nnz;

	for(int j = 0; j < dim_I; j++) {
		for(int i = 0; i < B_k.nnz; i++) {
			int index = i + j*B_k.nnz;
			int col = B_k.csrColIndA[i] + j*k;

			ret.csrColIndA[i + j*B_k.nnz] = B_k.csrColIndA[i] + j*k;
			ret.csrValA[i + j*B_k.nnz] = B_k.csrValA[i]; //might not be integer index
		}
	}
	return ret;
}

__global__ void bit_reversal(cuComplex * d_row, cuComplex *d_reversed, int n) {
	int i = threadIdx.x  + blockIdx.x  + blockDim.x;

	unsigned long x = i;
	if(i < n) {
		x = (((x & 0xaaaaaaaa) >> 1)|((x & 0x55555555) << 1));
		x = (((x & 0xcccccccc) >> 2)|((x & 0x33333333) << 2));
		x = (((x & 0xf0f0f0f0) >> 4)|((x & 0x0f0f0f0f) << 4));
		x = (((x & 0xff00ff00) >> 8)|((x & 0x00ff00ff) << 8));
		d_reversed[((x >> 16) | (x << 16))] = d_row[i];
	}
}


void ufft_row(cuComplex * d_row, int n ) {

	cusparseMatDescr_t descrA = NULL;
	const cuComplex alpha = make_cuComplex(1.0, 0.0);
	const cuComplex beta = make_cuComplex(0.0, 0.0);
	cusparseHandle_t cusparse_handle = NULL;
	cudaStream_t stream = NULL;

	cudaStreamCreate(&stream);
	cusparseCreate(&cusparse_handle);
	cusparseSetStream(cusparse_handle, stream);
	cusparseCreateMatDescr(&descrA);
	cusparseSetMatIndexBase(descrA, CUSPARSE_INDEX_BASE_ZERO);
	cusparseSetMatType(descrA, CUSPARSE_MATRIX_TYPE_GENERAL);

	cuComplex * d_Ikbk_csrValA = NULL;
	int *  d_Ikbk_csrRowPtrA = NULL;
	int * d_Ikbk_csrColIndA = NULL;

	cuComplex * d_to_reverse = NULL;

	cudaMalloc((void**)&d_Ikbk_csrValA,sizeof(cuComplex)*n*2);
	cudaMalloc((void**)&d_Ikbk_csrRowPtrA,sizeof(cuComplex)*(n + 1));
	cudaMalloc((void**)&d_Ikbk_csrColIndA,sizeof(int)*n*2);


	cuComplex * d_dense_matrix = NULL;
	cudaMalloc((void**)&d_dense_matrix, sizeof(cuComplex)*n*n);

	cuComplex dense_matrix[n][n];

	cudaMalloc((void**)&d_to_reverse,sizeof(cuComplex)*n);
	cuComplex * out = (cuComplex *) malloc(sizeof(cuComplex)*n);
	int num_blocks = ceil((float) n/32);
//	cudaMemcpy(d_to_reverse, d_row, sizeof(cuComplex)*n, cudaMemcpyDeviceToDevice);
//

		for (int k = 2; k < n; k*=2) {
//			construct B_k
			csr_complex_Mat B_k = generate_B_k_csr(k);
			csr_complex_Mat I_kron_B_k = kronecker_I_B_k(n, k, B_k);
			cudaMemcpy(d_Ikbk_csrValA, I_kron_B_k.csrValA, sizeof(cuComplex)*I_kron_B_k.nnz,cudaMemcpyHostToDevice);
			cudaMemcpy(d_Ikbk_csrRowPtrA, I_kron_B_k.csrRowPtrA, sizeof(int)*(n+1),cudaMemcpyHostToDevice);
			cudaMemcpy(d_Ikbk_csrColIndA, I_kron_B_k.csrColIndA, sizeof(int)*I_kron_B_k.nnz,cudaMemcpyHostToDevice);

//			cusparseCcsr2dense(cusparse_handle,
//								n,
//								n,
//								descrA,
//								d_Ikbk_csrValA,
//								d_Ikbk_csrRowPtrA,
//								d_Ikbk_csrColIndA,
//								d_dense_matrix,
//								n);
//
//			cudaMemcpy(dense_matrix, d_dense_matrix, sizeof(cuComplex)*n*n, cudaMemcpyDeviceToHost);
//			for(int i = 0; i < n; i++) {
//				for(int j =0; j < n; j++) {
//					std::cout << std::fixed << std::setprecision(1);
//					std::cout << "(" << dense_matrix[i][j].x << "," << dense_matrix[i][j].y << ") ";
//				}
//				std::cout << std::endl;
//			}
//			std::cout << std::endl;

			//compute x = I_kron_B_k*x
			cusparseCcsrmv(cusparse_handle,
							CUSPARSE_OPERATION_NON_TRANSPOSE,
							n,
							n,
							I_kron_B_k.nnz,
							&alpha,
							descrA,
							d_Ikbk_csrValA,
							d_Ikbk_csrRowPtrA,
							d_Ikbk_csrColIndA,
							d_row,
							&beta,
							d_row);
			cudaStreamSynchronize(stream);

		}
			csr_complex_Mat B_k = generate_B_k_csr(n);
//			csr_complex_Mat I_kron_B_k = kronecker_I_B_k(n,n, B_k);
			cudaMemcpy(d_Ikbk_csrValA, B_k.csrValA, sizeof(cuComplex)*B_k.nnz,cudaMemcpyHostToDevice);
			cudaMemcpy(d_Ikbk_csrRowPtrA, B_k.csrRowPtrA, sizeof(int)*(n+1),cudaMemcpyHostToDevice);
			cudaMemcpy(d_Ikbk_csrColIndA, B_k.csrColIndA, sizeof(int)*B_k.nnz,cudaMemcpyHostToDevice);

//			cusparseCcsr2dense(cusparse_handle,
//								n,
//								n,
//								descrA,
//								d_Ikbk_csrValA,
//								d_Ikbk_csrRowPtrA,
//								d_Ikbk_csrColIndA,
//								d_dense_matrix,
//								n);
//
//			cudaMemcpy(dense_matrix, d_dense_matrix, sizeof(cuComplex)*n*n, cudaMemcpyDeviceToHost);
//			for(int i = 0; i < n; i++) {
//				for(int j =0; j < n; j++) {
//					std::cout << std::fixed << std::setprecision(1);
//					std::cout << "(" << dense_matrix[i][j].x << "," << dense_matrix[i][j].y << ") ";
//				}
//				std::cout << std::endl;
//			}
//			std::cout << std::endl;
//			for(int i = 0; i < B_k.nnz; i++) {
//				std::cout << B_k.csrColIndA[i] << "," ;
//
//			}
//			std::cout << std::endl;
			//compute x = I_kron_B_k*x
			cusparseCcsrmv(cusparse_handle,
							CUSPARSE_OPERATION_TRANSPOSE,
							n,
							n,
							B_k.nnz,
							&alpha,
							descrA,
							d_Ikbk_csrValA,
							d_Ikbk_csrRowPtrA,
							d_Ikbk_csrColIndA,
							d_row,
							&beta,
							d_to_reverse);
		cudaStreamSynchronize(stream);
//		bit_reversal<<<num_blocks,32>>>(d_to_reverse, d_row, n);

		cudaMemcpy(d_row, d_to_reverse, sizeof(cuComplex)*n, cudaMemcpyDeviceToDevice);
//		for(int i = 0; i < n; i++) {
//			std::cout << out[i].x << ", ";
//		}
//		std::cout << std::endl;

		cudaFree(d_Ikbk_csrColIndA);
		cudaFree(d_Ikbk_csrRowPtrA);
		cudaFree(d_Ikbk_csrValA);
		cusparseDestroy(cusparse_handle);
		cudaStreamDestroy(stream);
		cusparseDestroyMatDescr(descrA);
}

void cuda_ufft(Complex *input_image, Complex * output_transform, InputImage input_image_meta) {
	cuComplex * d_input_image = NULL;
	cuComplex * d_output_transform = NULL;
	cuComplex * d_input_image_transpose = NULL;

	int w = input_image_meta.get_width();
	int h = input_image_meta.get_height();

	cuComplex * out = (cuComplex *) malloc(sizeof(cuComplex)*h*w);
	cudaMalloc((void**)&d_input_image, sizeof(cuComplex)*w*h);
	cudaMalloc((void**)&d_output_transform, sizeof(cuComplex)*w*h);
	cudaMalloc((void**)&d_input_image_transpose, sizeof(cuComplex)*w*h);

	int grid_height = ceil((float)h/THREADS_PER_BLOCK_SIDE);
	int grid_width = ceil((float)h/THREADS_PER_BLOCK_SIDE);


	cuComplex h_input_image[w*h];
	for(int i = 0; i < w*h; i++) {
		h_input_image[i] = make_cuComplex(input_image[i].real, input_image[i].imag);
	}
	cudaMemcpy(d_input_image, h_input_image, sizeof(cuComplex)*w*h, cudaMemcpyHostToDevice);


	for(int p = 0; p < h; p++) {
		ufft_row(d_input_image + p*w, w);

	}

	cudaMemcpy(out, d_input_image, sizeof(cuComplex)*w*h, cudaMemcpyDeviceToHost);
	for(int i = 0; i < w; i++) {
		std::cout << out[i].x << ", ";
	}
	std::cout << std::endl;

	transpose<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_input_image, d_input_image_transpose, w, h);

	for(int p = 0; p < h; p++) {
		ufft_row(d_input_image_transpose + p*w, w);
	}

	transpose<<<dim3(grid_width,grid_height), dim3(THREADS_PER_BLOCK_SIDE, THREADS_PER_BLOCK_SIDE)>>>(d_input_image_transpose, d_output_transform, w, h);



	cudaMemcpy(out, d_output_transform, sizeof(cuComplex)*w*h, cudaMemcpyDeviceToHost);
//	for(int i = 0; i < w*h; i++) {
//		std::cout << out[i].x << ", ";
//	}
//	std::cout << std::endl;
//	cudaMemcpy(h_input_image, d_output_transform, sizeof(cuComplex)*w*h,cudaMemcpyDeviceToHost);
	for(int i =0; i < w*h; i++) {
		output_transform[i].real = out[i].x;
		output_transform[i].imag = out[i].y;
	}
	cudaDeviceReset();
//	for(int i = 0; i < w*h; i++) {
//		std::cout << output_transform[i].real << ", ";
//	}
//	std::cout << std::endl;
}

