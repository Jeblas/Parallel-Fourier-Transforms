#include "./Multi_Threaded/threads_dtf.cc"
#include "./MPI/mpi_fft.h"
#include "Cuda/cuda_naive.h"

#include <iostream>

int main(int argc, char **argv) {
    // check for correct number of arguments
    if (argc != 4) {
        std::cout << "Incorrect number of arguments:";
        std::cout << " (forward|reverse) <input_file.txt> <output_file.txt\n>";
        return -1;
    }
    //define tags for the variants
    if (argv[1] == "forward") {
        // main_multi_forward(argc, argv);
        // main_mpi_forward(argc, argv);
        // main_cuda_forward(argc, argv);
    } else {
        // main_multi_reverse(argc, argv);
        // main_mpi_reverse(argc, argv);
        // main_cuda_reverse(argc, argv);
    }

    return 0;
}