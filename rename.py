import os

def rename_files(folder_path):
    counter = 1
    for filename in os.listdir(folder_path):
        if os.path.isfile(os.path.join(folder_path, filename)):
            # Extract the file extension
            _, extension = os.path.splitext(filename)

            # Create the new file name
            new_filename = f"image{counter}.png"

            # Rename the file
            os.rename(os.path.join(folder_path, filename), os.path.join(folder_path, new_filename))

            # Increment the counter
            counter += 1

if __name__ == "__main__":
    folder_path = "./images/input/"  # Replace with the actual path to your folder
    rename_files(folder_path)