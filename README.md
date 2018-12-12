# Build and executing
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./p31 <forward|reverse> <input_file.txt> <output_file.txt>
$ mpirun -np 8 ./p32 <forward|reverse> <input_file.txt> <output_file.txt> 

```

# Overview
P31 utilizes c++ threads
p32 utilizes MPI and requires OpenMPI or an equivalent
p33 utilizes Cuda and requires Cuda and GCC5.3

# TODO
- correct inverse fourier transform implementations
- parallelize Transforms and ifft loops
- check for edge case (# of threads > lines in input file)
- test non square inputs
