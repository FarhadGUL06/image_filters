build:
	g++ -std=c++11 -o serial serial.cpp image_filters.hpp ppm_io.cpp

run:
	./serial ./images/ppm/flower1.ppm ./images/output

clean:
	rm -f serial
