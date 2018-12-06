cmake_minimum_required(VERSION 3.9)
project(P3)

find_package(CUDA)
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.3)
    message(FATAL_ERROR "Require at least gcc-5.3")
endif()
if(${CUDA_FOUND})
    project(P3 LANGUAGES CXX CUDA)

    set(CMAKE_CXX_STANDARD 11)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -std=c++11; )
    set(SOURCE_FILES
            src/Cuda/cuda_naive.cu
#            src/MPI/mpi_fft.cc
#            src/Single_Threaded/dft.cc
#            src/Single_Threaded/recursive-fft.cc
            src/complex.cc
            src/input_image.cc
            src/Single_Threaded/single_threaded_naive.cpp)
    add_definitions(-DWITH_CUDA)
    include_directories(${CUDA_INCLUDE_DIRS})
    link_directories(${CUDA_LIBRARY_DIRS})
else(${CUDA_FOUND})
    set(SOURCE_FILES
            src/Cuda/cuda_naive.cu
#            src/MPI/mpi_fft.cc
#            src/Single_Threaded/dft.cc
#            src/Single_Threaded/recursive-fft.cc
            src/complex.cc
            src/input_image.cc src/main.cpp
            src/Single_Threaded/single_threaded_naive.cpp)
endif(${CUDA_FOUND})




#add_definitions(-DCUDA)
#add_definitions(-DMPI)
#add_definitions(-DTHREADS)
#add_definitions(-DTHREAD_SUBMISSION)
#add_definitions(-DDEV)

#find_package(Thrust REQUIRED)


add_executable(p3 ${SOURCE_FILES} main.cpp)

if(${CUDA_FOUND})
    target_link_libraries(p3 ${CUDA_LIBRARIES} ${CUDA_cusparse_LIBRARY} ${CUDA_cublas_LIBRARY} ${MPI_LIBRARIES})
elseif(${CUDA_FOUND})
    target_link_libraries(p3 ${MPI_LIBRARIES})
endif(${CUDA_FOUND})