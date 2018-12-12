#include "threads_dft.h"

const float PI = 3.14159265358979f;
const int NUM_THREADS = 8;

/*
void dft(Complex *input, Complex *output, int size) {
    for(int n = 0; n < size; ++n) {
        output[n] = Complex(0,0);
        for(int k = 0; k < size; ++k) {
            float theta = 2 * PI * n * k / size;
            Complex W = Complex(cos(theta), -sin(theta));
            output[n] = output[n] + W * input[k];
        }
    }
}
*/

void thread_dft_loop(Complex *input, Complex *output, int chunk_size, int thread_id, int num_cols) {
    int offset = thread_id * chunk_size * num_cols;
    for (int row = 0; row < chunk_size; row++) {
        dft(input + offset + row * num_cols, output + offset + row * num_cols, num_cols);
    }
}

void launch_threads_dft(std::vector<std::thread> & threads, Complex *input, Complex *output, int num_rows, int num_cols) {
    int chunk_size = num_rows / NUM_THREADS;
    for (int thread_num = 0; thread_num < NUM_THREADS - 1; ++thread_num) {
        threads[thread_num] = std::thread(thread_dft_loop, input, output, chunk_size, thread_num, num_cols);
    }
    chunk_size = num_rows - ((num_rows / NUM_THREADS) * (NUM_THREADS - 1));
    threads.back() = std::thread(thread_dft_loop, input, output, chunk_size, NUM_THREADS - 1, num_cols);

    for (int thread_num = 0; thread_num < threads.size(); ++thread_num) {
        threads[thread_num].join();
    }
}

int main(int argc, char **argv) {
    Complex *img;
    Complex *img_transpose;
    int img_width;
    int img_height;
    std::vector<std::thread> threads(NUM_THREADS);

    InputImage image_handler(argv[2]);
    img = image_handler.get_image_data();
    img_width = image_handler.get_width();
    img_height = image_handler.get_height();

    img_transpose = new Complex[img_width * img_height];
    Complex *out = new Complex[img_width * img_height];

    launch_threads_dft(threads, img, out, img_height, img_width);

    // Transpose
    for (int row = 0; row < img_height; ++row) {
        for (int col = 0; col < img_width; ++col) {
	        img_transpose[row + (col * img_height)] = out[col + (row * img_width)];
	    }
    }

    launch_threads_dft(threads, img_transpose, out, img_width, img_height);
    
    image_handler.save_image_data(argv[3], out, img_width, img_height);
    delete [] img_transpose;
    delete [] out;
    
    return 0;
}
