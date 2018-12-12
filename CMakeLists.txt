cmake_minimum_required(VERSION 3.9)
project(P3)

find_package(CUDA)
find_package(MPI REQUIRED)
find_package(Threads REQUIRED)


set(SOURCE_FILES
        src/input_image.cc
        src/fourier_transforms.cc
        src/complex.cc
        src/Single_Threaded/single_threaded_naive.cpp
        src/Single_Threaded/dft.cc
        src/Single_Threaded/recursive_fft.cc)
set(SOURCE_FILES_CUDA ${SOURCE_FILES})
set(SOURCE_FILES_MPI ${SOURCE_FILES})

set(SOURCE_FILES_MT ${SOURCE_FILES} src/Multi_Threaded/threads_dft.cc)
set(SOURCE_FILES ${SOURCE_FILES_MT})
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.3)
    message(FATAL_ERROR "Require at least gcc-5.3")
endif()

if(${MPI_FOUND})
    include_directories(${MPI_INCLUDE_PATH})
    set(SOURCE_FILES ${SOURCE_FILES}
            src/MPI/mpi_fft.cc)
    set(SOURCE_FILES_MPI ${SOURCE_FILES_MPI}
            src/MPI/mpi_fft.cc)
endif()

if(${CUDA_FOUND})
    project(P3 LANGUAGES CXX CUDA)
    set(CMAKE_CXX_STANDARD 11)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -std=c++11; )
    set(SOURCE_FILES ${SOURCE_FILES}
            src/Cuda/cuda_naive.cu)
    set(SOURCE_FILES_CUDA ${SOURCE_FILES_CUDA}
            src/Cuda/cuda_naive.cu)
    add_definitions(-DWITH_CUDA)
    include_directories(${CUDA_INCLUDE_DIRS})
    link_directories(${CUDA_LIBRARY_DIRS})
endif(${CUDA_FOUND})

add_executable(p3 ${SOURCE_FILES} src/main.cc)
target_compile_definitions(p3 PRIVATE -DTEST=1)

add_executable(p31 ${SOURCE_FILES_MT} src/main.cc)
target_compile_definitions(p31 PRIVATE -DMT=1)
target_link_libraries(p31 Threads::Threads)

if(${CUDA_FOUND})

    add_executable(p33 ${SOURCE_FILES_CUDA} src/main.cc)
    target_compile_definitions(p33 PRIVATE -DCUDA=1)
    target_link_libraries(p33 ${CUDA_LIBRARIES} ${CUDA_cusparse_LIBRARY} ${CUDA_cublas_LIBRARY})
    target_link_libraries(p3 ${CUDA_LIBRARIES} ${CUDA_cusparse_LIBRARY} ${CUDA_cublas_LIBRARY})
endif(${CUDA_FOUND})

if(${MPI_FOUND})
    add_executable(p32 ${SOURCE_FILES_MPI} src/main.cc)
    target_compile_definitions(p32 PRIVATE -DWMPI=1)
    target_link_libraries(p32 ${MPI_LIBRARIES})
    target_link_libraries(p3 ${MPI_LIBRARIES})
endif(${MPI_FOUND})