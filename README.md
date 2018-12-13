# Build and Execute
Automatic 
```bash
$ ./launch.sh -t
```

Manual
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./p31 <forward|reverse> <input_file.txt> <output_file.txt>
$ mpirun -np 8 ./p32 <forward|reverse> <input_file.txt> <output_file.txt> 
$ ./p33 <forward|reverse> <input_file.txt> <output_file.txt>
# **For Cuda analysis**
$ nvprof ./p33 <forward|reverse> <input_file.txt> <output_file.txt>   
```

# Overview
- p31 utilizes C++ threads
- p32 utilizes MPI (requires OpenMPI or an equivalent)
- p33 utilizes Cuda (requires Cuda and GCC 5.3)

# Future Improvements
- Parallelize transforms
- Test non square inputs (optional)
