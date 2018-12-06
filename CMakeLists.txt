cmake_minimum_required(VERSION 3.8)
project(mpi)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

include_directories(
        src
	${MPI_INCLUDE_PATH}
)

set(SOURCE_FILES
	src/input_image.cc
	src/complex.cc
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} src/MPI/mpi_fft.cc)
target_link_libraries(${PROJECT_NAME} ${MPI_LIBRARIES})

if(MPI_COMPILE_FLAGS)
    set_target_properties(${PROJECT_NAME} PROPERTIES
	    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
	set_target_properties(${PROJECT_NAME} PROPERTIES
	    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
