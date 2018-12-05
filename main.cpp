//
// Created by jbm on 12/4/18.
//

#include "src/complex.h"
#include "src/input_image.h"
#include "src/single_thread_naive.h"
#include "src/cuda_naive.h"
#include <iomanip>

int main(int argc, char * argv[]) {
	InputImage image_handler(argv[2]);
	Complex * input_image = image_handler.get_image_data();
	Complex * out_image = (Complex *) malloc(sizeof(Complex)*image_handler.get_width()*image_handler.get_height());
	Complex * out2_image = (Complex *) malloc(sizeof(Complex)*image_handler.get_width()*image_handler.get_height());
	single_thread_naive(input_image, out_image, image_handler);

	cuda_naive(input_image, out2_image, image_handler);
	for(int i = 0; i < image_handler.get_height(); i ++){
		for (int j = 0; j < image_handler.get_height(); j++) {
			std::cout << std::fixed << std::setprecision(1);
			std::cout << out_image[i* image_handler.get_width() + j] - out2_image[i* image_handler.get_width() + j] << ", ";
		}
		std::cout << std::endl;
	}

	image_handler.save_image_data(argv[3], out_image, image_handler.get_width(), image_handler.get_height());

}
