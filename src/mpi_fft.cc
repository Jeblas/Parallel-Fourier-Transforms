#include "complex.h"
#include "input_image.h"

#include <iostream>
#include <cmath>
#include <mpi.h>

const float PI = 3.14159265358979f;

void compose_complex_array(Complex *output, float *real, float *imag, size) {
    for (int i = 0; i < size; ++i) {
        output[i] = Complex(real[i], imag[i]);
    }
}

void decompose_complex_array(Complex *input, float *out_real, float *out_imag, size) {
    for (int i = 0; i < size; ++i) {
        out_real[i] = input[i].real;
        out_imag[i] = input[i].imag;
    }
}

void separate(Complex *array, size_t size) {
    // All evens to lower half odds to upper half
    Complex *temp = new Complex[size];

    for (size_t i = 0; i < (size >> 1); ++i) {
        temp[i] = array[(i << 1) + 1];
    }
    for (size_t i = 0; i < (size >> 1); ++i) {
        array[i] = array[i << 1];
    }
    for (size_t i = 0; i < (size >> 1); ++i) {
        array[i + (size >> 1)] = temp[i];
    }
    delete[] temp;
}

void fft(Complex *input, int size) {
    if (size <= 1) {
        return;
    }

    //divide
    separate(input, size);

    //recursive call
    fft(input, size >> 1);
    fft(input + (size >> 1), size >> 1);

    //combine
    for (size_t i = 0; i < size / 2; ++i) {
        float theta = -2 * PI * i / size;
	    Complex even = input[i];
	    Complex odd = input[i + (size >> 1)];
        
	    Complex twiddle_factor = Complex(cos(theta), sin(theta)) * odd;
        input[i] = even + twiddle_factor;
        input[i + (size >> 1)] = even - twiddle_factor;
    }
}

// TODO requires memcpy for rank 0
void distribute_mpi_data(int MPI_rank, int MPI_num_ranks, int chunk_size, int img_width, int img_height, Complex *img, Complex *elements) {
    if (MPI_rank == 0) {
    // TODO !!! memcopy first elements from img to elements ////////////////////

        // chunk array accordingly and send
        // all ranks except last one
        size_t send_size = chunk_size * img_width;
        float *out_real = new float[send_size];
        float *out_imag = new float[send_size];
        size_t index_offset;
        for (int rank = 1; rank < MPI_num_ranks - 1; ++rank) {
            index_offset = rank * send_size;
            decompose_complex_array(img + index_offset, out_real, out_imag, send_size);
            MPI_Send(out_real, send_size, MPI_FLOAT, rank, 0, MPI_COMM_WORLD);
            MPI_Send(out_imag, send_size, MPI_FLOAT, rank, 0, MPI_COMM_WORLD);
        }
        delete [] out_real;
        delete [] out_imag;

        index_offset = (MPI_num_ranks - 1) * img_width * chunk_size;
        send_size = (img_height - ((img_height / MPI_num_ranks) * (MPI_num_ranks - 1))) * img_width;
        out_real = new float[send_size];
        out_imag = new float[send_size];
        decompose_complex_array(img + index_offset, out_real, out_imag, send_size);
        MPI_Send(out_real, send_size, MPI_FLOAT, MPI_num_ranks - 1, 0, MPI_COMM_WORLD);
        MPI_Send(out_imag, send_size, MPI_FLOAT, MPI_num_ranks - 1, 0, MPI_COMM_WORLD);
        delete [] out_real;
        delete [] out_imag;

    } else {
        size_t recv_size = chunk_size * img_width;
        float *real_elements = new float[recv_size];
        float *imag_elements = new float[recv_size];
        MPI_Recv(real_elements, recv_size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(imag_elements, recv_size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        compose_complex_array(elements, real_elements, imag_elements, recv_size);
        delete [] real_elements;
        delete [] imag_elements;
    }
}

// TODO requires memcoy for rank 0
void collect_mpi_data(int MPI_rank, int MPI_num_ranks, int chunk_size, int img_width, int img_height, Complex *img, Complex *elements) {
    if (MPI_rank == 0) {
        //std::memcopy(); into img

    	size_t recv_size = chunk_size * img_width;
        float *real_elements = new float[recv_size];
        float *imag_elements = new float[recv_size];
        size_t index_offset;
        for (int rank = 1; rank < MPI_num_ranks - 1; ++rank) {
            index_offset = rank * recv_size;
            MPI_Recv(real_elements, recv_size, MPI_FLOAT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(imag_elements, recv_size, MPI_FLOAT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            compose_complex_array(img + index_offset , real_elements, imag_elements, recv_size);
        }
        delete [] real_elements;
        delete [] imag_elements;

        index_offset = (MPI_num_ranks - 1) * img_width * chunk_size;
        recv_size = (img_height - ((img_height / MPI_num_ranks) * (MPI_num_ranks - 1))) * img_width;
        real_elements = new float[recv_size];
        imag_elements = new float[recv_size];
        MPI_Recv(real_elements, recv_size, MPI_FLOAT, MPI_num_ranks - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(imag_elements, recv_size, MPI_FLOAT, MPI_num_ranks - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        compose_complex_array(img + index_offset , real_elements, imag_elements, recv_size);
        delete [] real_elements;
        delete [] imag_elements;


	} else {
        size_t send_size = chunk_size * img_width;
        float *out_real = new float[send_size];
        float *out_imag = new float[send_size];

        decompose_complex_array(elements, out_real, out_imag, send_size);
        MPI_Send(out_real, send_size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(out_imag, send_size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);

        delete [] out_real;
        delete [] out_imag;
    }
}

int main(int argc, char **argv) {
    // Need img_width, img_height
    MPI_Init(&argc, &argv);
    int MPI_num_ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &MPI_num_ranks);
    int MPI_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &MPI_rank);
    
    Complex *img;
    Complex *img_transpose;
    Complex *elements;
    int img_width;
    int img_height;
    uint32_t chunk_size;

    // Rank 0 responsible for reading file and sending values
    if (MPI_rank == 0) {
        InputImage image_handler(argv[2]);
        img = image_handler.get_image_data();
        img_width = image_handler.get_width();
        img_height = image_handler.get_height();
        
    }
    MPI_Bcast(&img_width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&img_height, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Barrier(MPI_COMM_WORLD);
    
    // Last rank can have a chunk size different than the rest for rows % ranks != 0
    if (MPI_rank != MPI_num_ranks - 1) {
        chunk_size = img_height / MPI_num_ranks;
    } else {
        chunk_size = img_height - ((img_height / MPI_num_ranks) * (MPI_num_ranks - 1));
    }

    elements = new Complex[img_width * chunk_size];

    //////////////////      ROWS        ////////////////// 
    distribute_mpi_data(MPI_rank, MPI_num_ranks, chunk_size, img_width, img_height, img, elements);
    for (int row = 0; row < chunk_size; ++row) {
	    fft(elements + (row * img_width), img_width);
    }
    collect_mpi_data(MPI_rank, MPI_num_ranks, chunk_size, img_width, img_height, img, elements);

    //////////////////      Transpose       ////////////////// 
    if (MPI_rank == 0) {
        img_transpose = new Complex[img_width * img_height];
        for (int row = 0; row < img_height; ++row) {
            for (int col = 0; col < img_width; ++col) {
                img_transpose[row + (col * img_height)] = img[col + (row * img_width)];
            }
        }
    }

    //////////////////      COLUMNS         ////////////////// 
    distribute_mpi_data(MPI_rank, MPI_num_ranks, chunk_size, img_height, img_width, img_transpose, elements);
    for (int row = 0; row < chunk_size; ++row) {
	    fft(elements + (row * img_height), img_height);
    }
    collect_mpi_data(MPI_rank, MPI_num_ranks, chunk_size, img_height, img_width, img_transpose, elements);
    delete [] elements;
    // Output
    if (MPI_rank == 0) {
        image_handler.save_image_data(argv[3], img_transpose, img_width, img_height);

        delete [] img_transpose;
    }

    MPI_Finalize();
}
