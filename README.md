# Build and Execute
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./p31 <forward|reverse> <input_file.txt> <output_file.txt>
$ mpirun -np 8 ./p32 <forward|reverse> <input_file.txt> <output_file.txt> 
$ ./p33 <forward|reverse> <input_file.txt> <output_file.txt>
$ nvprof ./p33 <forward|reverse> <input_file.txt> <output_file.txt>   **For analysis**

```

# Overview
- P31 utilizes C++ threads
- p32 utilizes MPI (requires OpenMPI or an equivalent)
- p33 utilizes Cuda (requires Cuda and GCC 5.3)

# TODO
- correct inverse fourier transform implementations
- parallelize Transforms and ifft loops
- check for edge case (# of threads > lines in input file)
- test non square inputs
