#ifdef MT
#include "Multi_Threaded/threads_dft.h"
#endif

#ifdef MPI
#include "MPI/mpi_fft.h"
#endif

#ifdef CUDA
#include "Cuda/cuda_naive.h"
#include "input_image.h"
#include "complex.h"

#endif

#include <iostream>

int main(int argc, char **argv) {
    // check for correct number of arguments
    if (argc != 4) {
        std::cout << "Incorrect number of arguments:";
        std::cout << " (forward|reverse) <input_file.txt> <output_file.txt\n>";
        return -1;
    }
    //define tags for the variants
#ifdef CUDA
FourierDirection direction;
if (!strcmp(argv[argc - 3], "foward")) {
    direction  = FORWARD;
} else {
    direction = REVERSE;
}

InputImage image_handler(argv[argc - 2]);

int w = image_handler.get_width();
int h = image_handler.get_height();

Complex input_image[w * h];
memcpy(input_image, image_handler.get_image_data(), sizeof(Complex) * w * h);
Complex out_image_cuda[w * h];
cuda_2d_dft(input_image, out_image_cuda, image_handler, direction);
image_handler.save_image_data(argv[argc - 1], out_image_cuda, w, h);
#endif

    if (!strcmp(argv[1], "forward")) {
#ifdef MT
        mt_dft_2d(argc, argv, false);
#endif
#ifdef MPI
        mpi_fft_2d(argc, argv, false);
#endif
#ifdef CUDA
	
#endif
	// main_cuda_forward(argc, argv);
    } else {
#ifdef MT
        mt_dft_2d(argc, argv,false);
#endif
#ifdef MPI
        mpi_fft_2d(argc, argv, true);
#endif
        // main_cuda_reverse(argc, argv);
    }

    return 0;
}
