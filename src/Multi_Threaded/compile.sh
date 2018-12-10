#!/bin/bash
g++ threads_dft.cc -o multi ../input_image.cc ../complex.cc ../fourier_transforms.cc -lpthread
