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

```
apply.py -> run all steps from above using:

Usage: python3 apply.py <cpp_bin> <number_of_images>
Example: Usage: python3 apply.py serial 5

```

## Input

The program can be run by executing:

```
<cpp_bin> <number_of_images>
```

It will take the first <number_of_images> as input images.
Next, it will read the filters that should be applied to the images.
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



Read and write ppm file

[1] https://stackoverflow.com/questions/2693631/read-ppm-file-and-store-it-in-an-array-coded-with-c
[2] https://lodev.org/cgtutor/filtering.html
