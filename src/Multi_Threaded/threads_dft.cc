#include "threads_dft.h"

const float PI = 3.14159265358979f;
const int NUM_THREADS = 8;

void thread_dft_loop(Complex *input, Complex *output, int chunk_size, int thread_id, int num_cols, bool is_reverse) {
    int offset = thread_id * chunk_size * num_cols;
    if(is_reverse) {
		for (int row = 0; row < chunk_size; row++) {
			inverse_dft(input + offset + row * num_cols, output + offset + row * num_cols, num_cols);
		}
    } else {
    	for (int row = 0; row < chunk_size; row++) {
			dft(input + offset + row * num_cols, output + offset + row * num_cols, num_cols);
		}
    }
}

void launch_threads_dft(std::vector<std::thread> & threads, Complex *input, Complex *output, int num_rows, int num_cols, bool is_reverse) {
    int chunk_size = num_rows / NUM_THREADS;
    for (int thread_num = 0; thread_num < NUM_THREADS - 1; ++thread_num) {
        threads[thread_num] = std::thread(thread_dft_loop, input, output, chunk_size, thread_num, num_cols, is_reverse);
    }
    chunk_size = num_rows - ((num_rows / NUM_THREADS) * (NUM_THREADS - 1));
    threads.back() = std::thread(thread_dft_loop, input, output, chunk_size, NUM_THREADS - 1, num_cols, is_reverse);

    for (int thread_num = 0; thread_num < threads.size(); ++thread_num) {
        threads[thread_num].join();
    }
}

void mt_dft_2d(int argc, char **argv, bool is_reverse) {
    Complex *img;
    Complex *img_transpose;
    int img_width;
    int img_height;
    std::vector<std::thread> threads(NUM_THREADS);

    InputImage image_handler;
    image_handler.read_image_data(argv[argc - 2]);
    img = image_handler.get_image_data();
    img_width = image_handler.get_width();
    img_height = image_handler.get_height();

    for(int i = 0; i < img_height*img_width; i++) {
    	std::cout << img[i] << ", ";
    	if(!((i+1) % img_width))
    		std::cout <<std::endl;
    }
    img_transpose = new Complex[img_width * img_height];
    Complex *out = new Complex[img_width * img_height];


    launch_threads_dft(threads, img, out, img_height, img_width, is_reverse);

    // Transpose
    for (int row = 0; row < img_height; ++row) {
        for (int col = 0; col < img_width; ++col) {
	        img_transpose[row + (col * img_height)] = out[col + (row * img_width)];
	    }
    }

    launch_threads_dft(threads, img_transpose, out, img_width, img_height, is_reverse);
    
    // Transpose
    for (int row = 0; row < img_height; ++row) {
        for (int col = 0; col < img_width; ++col) {
	        img_transpose[row + (col * img_height)] = out[col + (row * img_width)];
	    }
    }
	if(is_reverse)
    	image_handler.save_image_data_real(argv[3], img_transpose, img_width, img_height);
    else
		image_handler.save_image_data(argv[3], img_transpose, img_width, img_height);

	delete [] img_transpose;
    delete [] out;
    
}
