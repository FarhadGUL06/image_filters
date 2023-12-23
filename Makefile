build:
	g++ -ggdb3 -std=c++11 -o serial serial.cpp image_filters.hpp image_filters.cpp utils.cpp ppm_io.cpp
	g++ -ggdb3 -o pthread pthread.cpp image_filters.hpp image_filters.cpp utils.cpp ppm_io.cpp
	g++ -ggdb3 -fopenmp -o openmp openmp.cpp image_filters.hpp image_filters.cpp utils.cpp ppm_io.cpp
	#nvcc -O2 -g -std=c++11 cuda.cu -o cuda
	mpic++ -g -Wall -ggdb3 -fno-omit-frame-pointer -o mpi mpi.cpp image_filters.hpp image_filters.cpp utils.cpp ppm_io.cpp -lm

run_serial:
	./serial $(ARGS)

run_pthread:
	./pthread $(ARGS)

run_openmp:
	./openmp $(ARGS)

run_cuda:
	./cuda $(ARGS)

run_mpi:
	mpirun -np 8 ./mpi $(ARGS)

clean:
	rm -f serial
	rm -f pthread
	rm -f openmp
	rm -f ./images/ppm_before/*
	rm -f ./images/ppm_after/*
	rm -f ./images/output/*
