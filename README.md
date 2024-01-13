# image_filters

## Images hierarchy

```
./images/
├── input               -> used for input images (png)
│   └── <file_name>.png
├── output              -> used for final images after filtering (png)
│   └── <file_name>.png
├── ppm_after           -> images after filtering in CPP (ppm)
│   └── <file_name>.ppm
└── ppm_before          -> images before filtering in CPP (ppm)
    └── <file_name>.ppm
```

## Program hierarchy

```
├── Makefile
├── README.md
├── apply.py -> apply CPP program with different purposes
├── converter.py -> converts PNG to PPM and PPM to PNG
├── filters -> filters used on images
├── graphics
│   |── images -> graphics for modifying number of images
│   └── filters -> graphics for modifying number of filters
├── image_filters.cpp -> all filter functions
├── image_filters.hpp -> function definitions and constants
├── mpi.cpp
├── openmp.cpp
├── ppm_io.cpp -> read / write an PPM image
├── pthread.cpp
├── rename.py -> renames an unlabed dataset to image0...X.png
├── serial.cpp
├── timer.py -> graphics generator based on time
└── utils.cpp -> generic applyfilter function used by all filters
```

## Converter usage:

```
python3 converter.py <type> <input_file> <output_file>

<type> <- png / ppm (output file type)

```

Steps:

1. Convert an image from png to ppm using:

```
python3 converter.py ppm images/input/<file_name>.png images/ppm_before/<file_name>.ppm
```

2. Filter image in CPP using:

```
<cpp_bin> images/ppm_before/<file_name>.ppm images/ppm_after/<file_name>.ppm
```

3. Convert image back from ppm to png
```
python3 converter.py png images/ppm_after/<file_name>.ppm images/output/<file_name>.png
```


## Use of apply.py


apply.py -> run all steps from above using:

Usage: python3 apply.py <cpp_bin> <number_of_images>


| CPP_BIN   | Description              |
| :-------: | :----------------------: |
| serial    | Serial program           |
| mpi       | MPI-based program        |
| pthread   | Pthread Program          |
| openmp    | OpenMP Program           |


Example usage: 

```
python3 apply.py serial 5 1
```

## Input

The program can be run by executing:

```
<cpp_bin> <number_of_images>
```

It will take the first <number_of_images> as input images.
Next, the number of threads / procceses and last, it will read 
the filters that should be applied to the images.
Input example:

```
3   -> number of filters
1   -> filter number 1
2   -> filter number 2
3   -> filter number 3
```

All filters will be applied to all images.

Filter - ID table:

| ID    | Filter Type              |
| :---: | :----------------------: |
| 0     | blur_PPM                 |
| 1     | gaussian_blur_PPM        |
| 2     | motion_blur_PPM          |
| 3     | sharpen_PPM              |
| 4     | extreme_sharpen_PPM      |
| 5     | find_edges_PPM           |
| 6     | emboss_PPM               |
| 7     | extreme_emboss_PPM       |


Convert any jpg image to png before:

```
rename 's/\.jpg$/.png/' *.jpg

```

## Run examples

```
cat ./filters/test00.txt | python apply.py serial 5 1
cat ./filters/test00.txt | python apply.py pthread 16 8
cat ./filters/test00.txt | python apply.py openmp 16 8
cat ./filters/test00.txt | python apply.py mpi 16 8
```

## Time Graphics generator

Example usage:

Usage: python3 timer.py <type> <cpp_bin> <number_of_images> <num_threads / procs>

| Type  | Description                                                                   |
| :---: | :---------------------------------------------------------------------------: |
| 1     | Basic analysis on different number of images on one implementation            |
| 2     | Basic analysis on different number of filters on one implementation           |
| 3     | Basic analysis on different number of images on all implementations           |
| 4     | Basic analysis on different number of filters on all implementations          |
| 5     | Run each best time for all implementations on different number of images      |
| 6     | Run each best time for all implementations on different number of filters     |

```
python3 timer.py 1 mpi 50 16 -> evaluation on image number
with the same filters using 16 processes

python3 timer.py 2 mpi 1 16 -> evaluation on one image 
with different number of filters using 16 processes.

python3 timer.py 3 <any> 50 <any> -> runs first for all implementations
and plot the results

python3 timer.py 4 <any> 1 <any> -> runs second for all implementations
and plot the results

python3 timer.py 5 <any> 50 <any> -> runs the first (for changing number of images) 
for best implementations

python3 timer.py 6 <any> 1 <any> -> runs the second (for changing number of filters) 
for best implementations

```



## References

[1] https://stackoverflow.com/questions/2693631/read-ppm-file-and-store-it-in-an-array-coded-with-c

[2] https://lodev.org/cgtutor/filtering.html

[3] https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.convolve2d.html

[4] https://www.kaggle.com/datasets/arnaud58/landscape-pictures
