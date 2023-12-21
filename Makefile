build:
	g++ -std=c++11 -o serial serial.cpp image_filters.hpp image_filters.cpp

run:
	./serial ./images/ppm/flower1.ppm ./images/output

clean:
	rm -f serial
	rm -f ./images/ppm_before/*
	rm -f ./images/ppm_after/*
	rm -f ./images/output/*
