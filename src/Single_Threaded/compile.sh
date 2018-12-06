#!/bin/bash
g++ dft.cc -o dft ../input_image.cc ../complex.cc
g++ recursive-fft.cc -o fft ../input_image.cc ../complex.cc
