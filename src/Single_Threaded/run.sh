#!/bin/bash
./dft forward ../../input_files/Tower256.txt ./output_dft_256.txt
./fft forward ../../input_files/Tower256.txt ./output_fft_256.txt
./f_tests
