cmake_minimum_required(VERSION 3.9)
project(Parallel-Fourier-Transforms)

find_package(CUDA)
find_package(MPI)
find_package(Threads)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS})


set(SOURCE_FILES
        src/input_image.cc
        src/fourier_transforms.cc
        src/complex.cc)

if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.3)
    message(FATAL_ERROR "Require at least gcc-5.3")
endif()

if(${THREADS_FOUND})
    set(SOURCE_FILES_MT ${SOURCE_FILES} src/Multi_Threaded/threads_dft.cc)
    add_executable(p31 ${SOURCE_FILES_MT} src/main.cc)
    target_compile_definitions(p31 PRIVATE -DMT=1)
    target_link_libraries(p31 ${CMAKE_THREAD_LIBS_INIT})
endif()

if(${MPI_FOUND})
    set(SOURCE_FILES_MPI ${SOURCE_FILES})
    set(SOURCE_FILES_MPI ${SOURCE_FILES_MPI}
            src/MPI/mpi_fft.cc)
    include_directories(${MPI_INCLUDE_PATH})
    add_executable(p32 ${SOURCE_FILES_MPI} src/main.cc)
    target_compile_definitions(p32 PRIVATE -DWMPI=1)
    target_link_libraries(p32 ${MPI_LIBRARIES})
endif(${MPI_FOUND})

if(${CUDA_FOUND})
    project(P3 LANGUAGES CXX CUDA)
    set(CMAKE_CXX_STANDARD 11)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -std=c++11; )
    set(SOURCE_FILES_CUDA ${SOURCE_FILES})
    set(SOURCE_FILES_CUDA ${SOURCE_FILES_CUDA}
            src/Cuda/cuda_naive.cu)
    add_definitions(-DWITH_CUDA)
    include_directories(${CUDA_INCLUDE_DIRS})
    link_directories(${CUDA_LIBRARY_DIRS})
    add_executable(p33 ${SOURCE_FILES_CUDA} src/main.cc)
    target_compile_definitions(p33 PRIVATE -DCUDA=1)
    target_link_libraries(p33 ${CUDA_LIBRARIES} ${CUDA_cusparse_LIBRARY} ${CUDA_cublas_LIBRARY})
endif(${CUDA_FOUND})