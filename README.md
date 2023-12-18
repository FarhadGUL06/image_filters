# image_filters

# Images hierarchy

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


# Converter usage:

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


# Use of apply.py

```
apply.py -> run all steps from above using:

Usage: python3 apply.py <cpp_bin> <file_name>

Example: Usage: python3 apply.py serial flower1

```



Read and write ppm file

[1] https://stackoverflow.com/questions/2693631/read-ppm-file-and-store-it-in-an-array-coded-with-c