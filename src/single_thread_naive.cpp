//
// Created by jbm on 12/4/18.
//

#include "single_thread_naive.h"





void single_thread_naive(Complex * input_image, Complex * output_transform, InputImage input_image_meta) {

//	use of M_PI below might cause numerical discrepancies

	int w = input_image_meta.get_width();
	int h = input_image_meta.get_height();
// calculate 1st step (dft of each row)
//	std::cout << input_image[w] << std::endl;
	for(int i = 0; i < h; i++){
		for(int j = 0; j < w; j++) {
			output_transform[i * w + j] = Complex(0, 0);
			for (int k = 0; k < w; k++) {
				Complex W = Complex(cos(2 * M_PI * j * k / w), -sin(
						2 * M_PI * j * k / w));
//				std::cout << W << std::endl;
				output_transform[i * w + j] =
						output_transform[i * w + j] +
						W * input_image[i * w + k];
			}
		}
	}
//	std::cout << output_transform[w] << std::endl;
	Complex * intermediate = (Complex *) malloc(sizeof(Complex)*input_image_meta.get_height()*input_image_meta.get_width());
	memcpy(intermediate, output_transform, sizeof(Complex)*input_image_meta.get_height()*input_image_meta.get_width());
//	calculate 2nd step (dft of each col)
	for(int i = 0; i < h; i++){
		for(int j = 0; j < w; j ++){
			output_transform[i*w + j] = Complex(0,0);
			for(int k = 0; k < h; k++) {
				Complex W = Complex(cos(2*M_PI*i*k/h), -sin(2*M_PI*i*k/h));
				output_transform[i*w + j] = output_transform[i*w + j] + W*intermediate[k*w + j];
			}
		}
	}

}
