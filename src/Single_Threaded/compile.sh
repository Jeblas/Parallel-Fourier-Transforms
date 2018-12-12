#!/bin/bash
g++ recursive_fft.cc -o fft ../input_image.cc ../complex.cc ../fourier_transforms.cc
g++ fourier_tests.cc -o f_tests ../input_image.cc ../complex.cc ../fourier_transforms.cc
g++ dft.cc -o dft ../input_image.cc ../complex.cc ../fourier_transforms.cc
