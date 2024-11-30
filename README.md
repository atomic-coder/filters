# Image Filtering Program

This program filters an image by applying a custom filter.

## How to Use:

1. **Clone the Repository**: 
   - If you haven't already, clone the repository to your local machine or open it directly in GitHub Codespaces.
   
2. **Set Up Your Environment**: 
   - Open a terminal inside GitHub Codespaces (or any terminal if you're using a local setup).
   - Make sure you have `gcc` installed in the environment. You can install it using `sudo apt install build-essential` if it’s not installed.

3. **Compile the Program**: 
   - Run the following command to compile the `filters.c` file:
     ```bash
     gcc -o filters filters.c -lm
     ```
     This will create an executable named `filters` (no `.exe` extension needed on Linux-based systems).

4. **Run the Program**:
   - Execute the program using:
     ```bash
     ./filters
     ```

5. **Follow the Instructions**: 
   - The program will prompt you to input an image (in `.bmp` format), select a filter, and specify an output file name.

6. **Save the Output**:
   - After applying the filter, the output image will be saved to your specified file name with a `.bmp` extension.

## Notes:
- The program applies basic filters like **grayscale**, **negative**, and others. You can modify the filter logic in `filters.c` for other types of image processing or add more filters.
- The program only supports `.bmp` image files as input and output.
