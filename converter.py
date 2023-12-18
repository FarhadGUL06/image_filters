from PIL import Image
import sys

def png_to_ppm(input_path, output_path):
    try:
        # Open the PNG image
        image = Image.open(input_path)
        # Convert RGBA to RGB if the image has an alpha channel
        if image.mode == 'RGBA':
            image = image.convert('RGB')
        # Save as PPM
        image.save(output_path, 'PPM')
        print(f"Conversion successful. Image saved to {output_path}")
    except Exception as e:
        print(f"Error: {e}")

def ppm_to_png(input_path, output_path):
    try:
        # Open the PPM image
        image = Image.open(input_path)
        # Save as PNG
        image.save(output_path, 'PNG')
        print(f"Conversion successful. Image saved to {output_path}")
    except Exception as e:
        print(f"Error: {e}")

def main():
    args = sys.argv[1:]
    conversion = args[0]
    input_file = args[1]
    output_file = args[2]
    if conversion == "ppm":
        png_to_ppm(input_file, output_file)
    elif conversion == "png":
        ppm_to_png(input_file, output_file)
    else:
        print("ppm for output as ppm image and png for output as png image")

if __name__ == '__main__':
    main()