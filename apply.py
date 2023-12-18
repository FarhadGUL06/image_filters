import subprocess
import os
import sys

def convert_png_to_ppm(input_path, output_path):
    subprocess.run(['python3', 'converter.py', 'ppm', input_path, output_path])

def filter_image_in_cpp(input_path, output_path, cpp_bin):
    subprocess.run([cpp_bin, input_path, output_path])

def convert_ppm_to_png(input_path, output_path):
    subprocess.run(['python3', 'converter.py', 'png', input_path, output_path])

def compile_cpp():
    subprocess.run(['make', 'clean'])
    subprocess.run(['make'])

def main():
    compile_cpp()
    input_folder = 'images/input/'
    ppm_before_folder = 'images/ppm_before/'
    ppm_after_folder = 'images/ppm_after/'
    output_folder = 'images/output/'
    
    if len(sys.argv) < 3:
        print("Usage: python3 apply.py <cpp_bin> <file_name>")
        sys.exit(1)
    
    args = sys.argv[1:]
    cpp_bin = args[0]
    
    # if cpp_bin does not start with ./, insert it
    if not cpp_bin.startswith('./'):
        cpp_bin = './' + cpp_bin
    
    file_name = args[1]

    # Ensure output directories exist
    os.makedirs(ppm_before_folder, exist_ok=True)
    os.makedirs(ppm_after_folder, exist_ok=True)
    os.makedirs(output_folder, exist_ok=True)

    # Replace 'file_name.png' with the actual file name
    

    # Step 1: Convert PNG to PPM
    png_input_path = os.path.join(input_folder, f'{file_name}.png')
    ppm_before_output_path = os.path.join(ppm_before_folder, f'{file_name}.ppm')
    convert_png_to_ppm(png_input_path, ppm_before_output_path)

    # Step 2: Filter image in CPP
    ppm_before_input_path = ppm_before_output_path
    ppm_after_output_path = os.path.join(ppm_after_folder, f'{file_name}.ppm')
    filter_image_in_cpp(ppm_before_input_path, ppm_after_output_path, cpp_bin)

    # Step 3: Convert PPM to PNG
    ppm_after_input_path = ppm_after_output_path
    png_output_path = os.path.join(output_folder, f'{file_name}.png')
    convert_ppm_to_png(ppm_after_input_path, png_output_path)

if __name__ == "__main__":
    main()