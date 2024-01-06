import subprocess
import os
import sys
import timeit

from matplotlib import pyplot as plt

colors = ["blue", "red", "green", "black", "purple", "orange", "cyan", "magenta", "yellow", "brown", "gray", "pink", "teal", "lime", "olive", "navy", "maroon", "gold", "indigo", "violet", "turquoise", "salmon", "skyblue", "peru", "thistle", "darkgreen", "orchid", "steelblue", "coral", "darkorange", "mediumseagreen", "dodgerblue", "mediumvioletred", "darkslategray", "darkkhaki", "mediumorchid", "cadetblue", "darkolivegreen", "slategray", "darkmagenta", "lightcoral", "deepskyblue", "mediumspringgreen", "darkcyan"]

no_images = [5, 10, 15, 25, 30, 40, 50]
no_filters = [2, 5, 6, 7, 10, 11, 13, 14, 16, 17]
no_threads = [4, 8, 16, 32, 64]


def convert_png_to_ppm(input_path, output_path):
    subprocess.run(['python3', 'converter.py', 'ppm', input_path, output_path])

def filter_image_in_cpp(cpp_bin, number_of_images):
    repeats = 1
    print("Running " + cpp_bin + " on " + number_of_images + " images" + " with " + num_threads + " threads")
    if cpp_bin != './mpi':
        result = timeit.timeit(lambda: subprocess.run([cpp_bin, number_of_images, num_threads]), number=repeats)
        result = result / repeats
    else:
        print("num_thread = ", num_threads)
        result = timeit.timeit(lambda: subprocess.run(['mpirun', '-np', num_threads, cpp_bin, number_of_images]), number=repeats)
        result = result / repeats
    print(f"Time taken for {cpp_bin} using {num_threads} threads: " + str("{0:.3f}".format(result)))
    return float("{0:.3f}".format(result))

def filter_image_in_cpp_diff_filters(cpp_bin, number_of_images, filter_file):
    repeats = 1
    print("Running " + cpp_bin + " on " + number_of_images + " images" + " with " + num_threads + " threads" + " and " + filter_file + " filter_file")
    if cpp_bin != './mpi':
        command = f"cat {filter_file} | {cpp_bin} {number_of_images} {num_threads}"
        print(command)
        result = timeit.timeit(lambda: subprocess.run(command, shell=True), number=repeats)
        result = result / repeats
    else:
        print("Num proc = ", num_threads)
        command = f"cat {filter_file} | mpirun -np {num_threads} --oversubscribe {cpp_bin} {number_of_images}"
        print(command)
        result = timeit.timeit(lambda: subprocess.run(command, shell=True), number=repeats)
        result = result / repeats
    print(f"Time taken for {cpp_bin} using {num_threads} threads: " + str("{0:.3f}".format(result)))
    return float("{0:.3f}".format(result))

def convert_ppm_to_png(input_path, output_path):
    subprocess.run(['python3', 'converter.py', 'png', input_path, output_path])

def compile_cpp():
    subprocess.run(['make', 'clean'])
    subprocess.run(['make'])

def all_png_to_ppm(input_folder, ppm_before_folder, number_of_images):
    for i in range(0, int(number_of_images)):
        file_name = f'image{i}'
        png_input_path = os.path.join(input_folder, f'{file_name}.png')
        ppm_before_output_path = os.path.join(ppm_before_folder, f'{file_name}.ppm')
        convert_png_to_ppm(png_input_path, ppm_before_output_path)

def all_ppm_to_png(ppm_after_folder, output_folder, number_of_images):
    for i in range(0, int(number_of_images)):
        file_name = f'image{i}'
        ppm_after_input_path = os.path.join(ppm_after_folder, f'{file_name}.ppm')
        png_output_path = os.path.join(output_folder, f'{file_name}.png')
        convert_ppm_to_png(ppm_after_input_path, png_output_path)


def create_graphic(indices, values, title, x_label, y_label, file_name):
    plt.plot(indices, values)
    plt.title(title)
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.savefig(file_name)
    plt.clf()
    plt.cla()
    plt.close()


def generate_graphic(indices, values, title, ox_label, oy_label, file_name, cpp_bin):
    # indices = [23, 40 .. ]
    # values = [[time_for_mpi_4], [time_for_mpi_8] .. ]
    length = len(values)
    for i in range(length):
        x = indices
        y = values[i]
        plt.plot(x, y, label = f"{cpp_bin}_{no_threads[i]}", linewidth='1.5', linestyle='solid', color=colors[i])
    plt.title(title)
    plt.xlabel(ox_label)
    plt.ylabel(oy_label)
    plt.grid(True)
    plt.legend()
    plt.show()
    plt.savefig(file_name)
    plt.close()

def generate_graphic_best(indices, values, title, ox_label, oy_label, file_name):
    # indices = [23, 40 .. ]
    # values = [[time_for_mpi_4], [time_for_mpi_8] .. ]
    length = len(values)
    for i in range(length):
        x = indices
        y = values[i]
        cpp_bin = list(dict_best.keys())[i]
        nrthrd = dict_best[cpp_bin]
        plt.plot(x, y, label = f"{cpp_bin}_{nrthrd}", linewidth='1.5', linestyle='solid', color=colors[i])
    plt.title(title)
    plt.xlabel(ox_label)
    plt.ylabel(oy_label)
    plt.grid(True)
    plt.legend()
    plt.show()
    plt.savefig(file_name)
    plt.close()

def analize_diff_images(cpp_bin):
    times_images = []
    for nimg in no_images:
        times_images.append(filter_image_in_cpp_diff_filters(cpp_bin, str(nimg), f'./filters/test00.txt'))
    os.makedirs('./graphics/images', exist_ok=True)
    create_graphic(no_images, times_images, "Time taken to filter images with " + cpp_bin, "Number of images", "Time (s)", f"./graphics/images/{cpp_bin}_{num_threads}.png")
    


def analize_diff_filters(cpp_bin, number_of_images):
    times_filters = []
    for i in range(0, len(no_filters)):
        times_filters.append(filter_image_in_cpp_diff_filters(cpp_bin, str(number_of_images), f'./filters/test0{i}.txt'))
    os.makedirs('./graphics/filters', exist_ok=True)
    create_graphic(no_filters, times_filters, "Time taken to filter images with " + cpp_bin, "Number of filters", "Time (s)", f"./graphics/filters/{cpp_bin}_{num_threads}.png")


def analize_multiple_images_implementation():
    cpp_bins = ['./mpi', './openmp', './pthread']
    for cpp_bin in cpp_bins:
        global num_threads
        times_images_all = []
        for nthreads in no_threads:
            times_images = []
            num_threads = str(nthreads)
            for nimg in no_images:
                times_images.append(filter_image_in_cpp_diff_filters(cpp_bin, str(nimg), f'./filters/test00.txt'))
            times_images_all.append(times_images)
        os.makedirs('./graphics/images', exist_ok=True)
        generate_graphic(no_images, times_images_all, "Time taken to filter images with " + cpp_bin, "Number of images", "Time (s)", f"./graphics/images/{cpp_bin}.png", cpp_bin)    

def analize_multiple_filters_implementation(number_of_images):
    cpp_bins = ['./mpi', './openmp', './pthread']
    for cpp_bin in cpp_bins:
        global num_threads
        times_filters_all = []
        for nthreads in no_threads:
            times_filters = []
            num_threads = str(nthreads)
            for i in range(0, len(no_filters)):
                times_filters.append(filter_image_in_cpp_diff_filters(cpp_bin, str(number_of_images), f'./filters/test0{i}.txt'))
            times_filters_all.append(times_filters)
        os.makedirs('./graphics/filters', exist_ok=True)
        generate_graphic(no_filters, times_filters_all, "Time taken to filter images with " + cpp_bin, "Number of filters", "Time (s)", f"./graphics/filters/{cpp_bin}.png", cpp_bin)


def best_time_multiple_images():
    cpp_bins = ['./mpi', './openmp', './pthread', './serial']
    global dict_best
    dict_best = {}
    dict_best.update({'./mpi': 16})
    dict_best.update({'./openmp': 64})
    dict_best.update({'./pthread': 64})
    dict_best.update({'./serial': 1})
    times_images_all = []
    for cpp_bin in cpp_bins:
        global num_threads
        times_images = []
        if cpp_bin == './mpi':
            num_threads = '16'
        elif cpp_bin == './openmp':
            num_threads = '64'
        elif cpp_bin == './pthread':
            num_threads = '64'
        else:
            num_threads = '1'
        for nimg in no_images:
            times_images.append(filter_image_in_cpp_diff_filters(cpp_bin, str(nimg), f'./filters/test00.txt'))
        times_images_all.append(times_images)
    os.makedirs('./graphics/images', exist_ok=True)
    generate_graphic_best(no_images, times_images_all, "Time taken to filter images", "Number of images", "Time (s)", "./graphics/images/best_time.png")


def best_time_multiple_filters(number_of_images):
    cpp_bins = ['./mpi', './openmp', './pthread', './serial']
    global dict_best
    dict_best = {}
    dict_best.update({'./mpi': 16})
    dict_best.update({'./openmp': 64})
    dict_best.update({'./pthread': 64})
    dict_best.update({'./serial': 1})
    times_filters_all = []
    for cpp_bin in cpp_bins:
        global num_threads
        times_filters = []
        if cpp_bin == './mpi':
            num_threads = '16'
        elif cpp_bin == './openmp':
            num_threads = '64'
        elif cpp_bin == './pthread':
            num_threads = '64'
        else:
            num_threads = '1'
        for i in range(0, len(no_filters)):
            times_filters.append(filter_image_in_cpp_diff_filters(cpp_bin, str(number_of_images), f'./filters/test0{i}.txt'))
        times_filters_all.append(times_filters)
    os.makedirs('./graphics/filters', exist_ok=True)
    generate_graphic_best(no_filters, times_filters_all, "Time taken to filter images", "Number of filters", "Time (s)", "./graphics/filters/best_time.png")

def main():
    compile_cpp()
    input_folder = 'images/input/'
    ppm_before_folder = 'images/ppm_before/'
    ppm_after_folder = 'images/ppm_after/'
    output_folder = 'images/output/'
    
    if len(sys.argv) < 5:
        print("Usage: python3 timer.py <type> <cpp_bin> <number_of_images> <num_threads / procs>")
        # type = 1 -> eval on different number of images
        # type = 2 -> eval on different number of filters
        sys.exit(1)
    
    args = sys.argv[1:]
    cpp_bin = args[1]

    global num_threads
    num_threads = args[3]
    
    # if cpp_bin does not start with ./, insert it
    if not cpp_bin.startswith('./'):
        cpp_bin = './' + cpp_bin
    
    number_of_images = args[2]

    # Type
    type = args[0]

    # Ensure output directories exist
    os.makedirs(ppm_before_folder, exist_ok=True)
    os.makedirs(ppm_after_folder, exist_ok=True)
    os.makedirs(output_folder, exist_ok=True)
    

    # Step 1: Convert PNG to PPM
    all_png_to_ppm(input_folder, ppm_before_folder, number_of_images)

    # Step 2: Filter image in CPP
    if type == '1':     # run basic analisys on one specific impl for nr images
        analize_diff_images(cpp_bin)
    elif type == '2':   # run basic analisys on one specific impl for nr filters
        analize_diff_filters(cpp_bin, number_of_images)
    elif type == '3':   # run basic analisys on all impl for nr images
        analize_multiple_images_implementation()
    elif type == '4':   # run basic analisys on all impl for nr filters
        analize_multiple_filters_implementation(number_of_images)
    elif type == '5':   # run best time for all impl for nr images
        best_time_multiple_images()
    elif type == '6':   # run best time for all impl for nr filters
        best_time_multiple_filters(number_of_images)
    
    # Step 3: Convert PPM to PNG
    all_ppm_to_png(ppm_after_folder, output_folder, number_of_images)

if __name__ == "__main__":
    main()