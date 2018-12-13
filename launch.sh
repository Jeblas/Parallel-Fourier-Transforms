#!/bin/bash

if [ ! -d "build" ]; then
    mkdir build
    cd build
    cmake ..
    make 
    cd ..
fi

if [ ! -d "output_files" ]; then
    mkdir output_files
fi

key="$1"

case $key in
    -t|--time)
        if [ -f "output_file/results.txt" ]; then
            rm output_file/results.txt
        fi

        echo " " > ./output_files/results.txt

        if [ -f "build/p31" ]; then 
            echo "=====================" >> ./output_files/results.txt
            echo "         MT          " >> ./output_files/results.txt
            echo "=====================" >> ./output_files/results.txt
            if [ -f "./input_files/TTower128.txt" ]; then
                echo "MT forward 128 " >> ./output_files/results.txt
                { time ./build/p31 forward ./input_files/TTower128.txt ./output_files/128_MT_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower256.txt" ]; then
                echo "MT forward 256 " >> ./output_files/results.txt
                { time ./build/p31 forward ./input_files/TTower256.txt ./output_files/256_MT_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower512.txt" ]; then
                echo "MT forward 512 " >> ./output_files/results.txt
                { time ./build/p31 forward ./input_files/TTower512.txt ./output_files/512_MT_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower1024.txt" ]; then
                echo "MT forward 1024 " >> ./output_files/results.txt
                { time ./build/p31 forward ./input_files/TTower1024.txt ./output_files/1024_MT_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower2048.txt" ]; then
                echo "MT forward 2048 " >> ./output_files/results.txt
                { time ./build/p31 forward ./input_files/TTower2048.txt ./output_files/2048_MT_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
        fi
        if [ -f "build/p32" ]; then
            echo "=====================" >> ./output_files/results.txt
            echo "         MPI         " >> ./output_files/results.txt
            echo "=====================" >> ./output_files/results.txt
            if [ -f "./input_files/TTower128.txt" ]; then
                echo "MPI forward 128 " >> ./output_files/results.txt
                { time mpirun -np 8 ./build/p32 forward ./input_files/TTower128.txt ./output_files/128_MPI_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower256.txt" ]; then
                echo "MPI forward 256 " >> ./output_files/results.txt
                { time mpirun -np 8 ./build/p32 forward ./input_files/TTower256.txt ./output_files/256_MPI_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower512.txt" ]; then
                echo "MPI forward 512 " >> ./output_files/results.txt
                { time mpirun -np 8 ./build/p32 forward ./input_files/TTower512.txt ./output_files/512_MPI_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower1024.txt" ]; then
                echo "MPI forward 1024 " >> ./output_files/results.txt
                { time mpirun -np 8 ./build/p32 forward ./input_files/TTower1024.txt ./output_files/1024_MPI_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower2048.txt" ]; then
                echo "MPI forward 2048 " >> ./output_files/results.txt
                { time mpirun -np 8 ./build/p32 forward ./input_files/TTower2048.txt ./output_files/2048_MPI_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
        fi
        if [ -f "build/p33" ]; then
            echo "=====================" >> ./output_files/results.txt
            echo "        Cuda         " >> ./output_files/results.txt
            echo "=====================" >> ./output_files/results.txt
            if [ -f "./input_files/TTower128.txt" ]; then
                echo "Cuda forward 128 " >> ./output_files/results.txt
                { time ./build/p33 forward ./input_files/TTower128.txt ./output_files/128_Cuda_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower256.txt" ]; then
                echo "Cuda forward 256 " >> ./output_files/results.txt
                { time ./build/p33 forward ./input_files/TTower256.txt ./output_files/256_Cuda_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower512.txt" ]; then
                echo "Cuda forward 512 " >> ./output_files/results.txt
                { time ./build/p33 forward ./input_files/TTower512.txt ./output_files/512_Cuda_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower1024.txt" ]; then
                echo "Cuda forward 1024 " >> ./output_files/results.txt
                { time ./build/p33 forward ./input_files/TTower1024.txt ./output_files/1024_Cuda_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi
            if [ -f "./input_files/TTower2048.txt" ]; then
                echo "Cuda forward 2048 " >> ./output_files/results.txt
                { time ./build/p33 forward ./input_files/TTower2048.txt ./output_files/2048_Cuda_forward.txt ; } 2>> ./output_files/results.txt
                echo "" >> ./output_files/results.txt
            fi 
        fi
        ;;
    -h|--help)
        echo "Script flags"
        echo "-t | --time         : stores execution time in output_files/results.txt"
        echo "-h | --help         : return this output"
        echo "-s | --silent       : executes programs without timing"
        echo "--test              : executes programs on input 8x8 (foward and reverse)"
        ;;
    -s|--silent)

        if [ -f "build/p31" ]; then 
            if [ -f "./input_files/TTower128.txt" ]; then
                ./build/p31 forward ./input_files/TTower128.txt ./output_files/128_MT_forward.txt
            fi
            if [ -f "./input_files/TTower256.txt" ]; then
                ./build/p31 forward ./input_files/TTower256.txt ./output_files/256_MT_forward.txt
            fi
            if [ -f "./input_files/TTower512.txt" ]; then
                ./build/p31 forward ./input_files/TTower512.txt ./output_files/512_MT_forward.txt
            fi
            if [ -f "./input_files/TTower1024.txt" ]; then
                ./build/p31 forward ./input_files/TTower1024.txt ./output_files/1024_MT_forward.txt
            fi
            if [ -f "./input_files/TTower2048.txt" ]; then
                ./build/p31 forward ./input_files/TTower2048.txt ./output_files/2048_MT_forward.txt
            fi
        fi 
        if [ -f "build/p32" ]; then 
            if [ -f "./input_files/TTower128.txt" ]; then
                mpirun -np 8 ./build/p32 forward ./input_files/TTower128.txt ./output_files/128_MPI_forward.txt
            fi
            if [ -f "./input_files/TTower256.txt" ]; then
                mpirun -np 8 ./build/p32 forward ./input_files/TTower256.txt ./output_files/256_MPI_forward.txt
            fi
            if [ -f "./input_files/TTower512.txt" ]; then
                mpirun -np 8 ./build/p32 forward ./input_files/TTower512.txt ./output_files/512_MPI_forward.txt
            fi
            if [ -f "./input_files/TTower1024.txt" ]; then
                mpirun -np 8 ./build/p32 forward ./input_files/TTower1024.txt ./output_files/1024_MPI_forward.txt
            fi
            if [ -f "./input_files/TTower2048.txt" ]; then
                mpirun -np 8 ./build/p32 forward ./input_files/TTower2048.txt ./output_files/2048_MPI_forward.txt
            fi
        fi
        if [ -f "build/p33" ]; then 
            if [ -f "./input_files/TTower128.txt" ]; then
                ./build/p33 forward ./input_files/TTower128.txt ./output_files/128_Cuda_forward.txt
            fi
            if [ -f "./input_files/TTower256.txt" ]; then
                ./build/p33 forward ./input_files/TTower256.txt ./output_files/256_Cuda_forward.txt
            fi
            if [ -f "./input_files/TTower512.txt" ]; then
                ./build/p33 forward ./input_files/TTower512.txt ./output_files/512_Cuda_forward.txt
            fi
            if [ -f "./input_files/TTower1024.txt" ]; then
                ./build/p33 forward ./input_files/TTower1024.txt ./output_files/1024_Cuda_forward.txt
            fi
            if [ -f "./input_files/TTower2048.txt" ]; then
                ./build/p33 forward ./input_files/TTower2048.txt ./output_files/2048_Cuda_forward.txt
            fi
        fi
        ;;
    --test)
        if [ -f "build/p31" ]; then 
            ./build/p31 forward ./input_files/Tower8.txt ./output_files/8_MT_forward.txt
            ./build/p31 reverse ./output_files/8_MT_forward.txt ./output_files/8_MT_reverse.txt
        fi
        if [ -f "build/p32" ]; then 
            mpirun -np 8 ./build/p32 forward ./input_files/Tower8.txt ./output_files/8_MPI_forward.txt
            mpirun -np 8 ./build/p32 reverse ./output_files/8_MPI_forward.txt ./output_files/8_MPI_reverse.txt
        fi
        if [ -f "build/p33" ]; then 
            ./build/p33 forward ./input_files/Tower8.txt ./output_files/8_Cuda_forward.txt
            ./build/p33 reverse ./output_files/8_Cuda_forward.txt ./output_files/8_Cuda_reverse.txt
        fi
        ;;
    *)
        echo "Requires a valid flag."
        echo "Type './launch.sh -h' for options."
        ;;
esac
