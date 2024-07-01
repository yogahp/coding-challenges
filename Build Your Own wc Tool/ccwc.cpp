#include <iostream>    // For standard input and output
#include <fstream>     // For file handling
#include <cstdio>      // For perror function
#include <cstdlib>     // For EXIT_FAILURE and EXIT_SUCCESS macros

// Function to count the number of bytes in a file
void count_bytes(const char* filename) {
  // Open the file in binary mode and move the file pointer to the end
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    // Print an error message if the file cannot be opened
    std::perror("Error opening file");
    return;
  }

  // Get the position of the file pointer, which represents the size of the file
  std::streamsize byte_count = file.tellg();
  file.close();  // Close the file

  if (byte_count == -1) {
    // Print an error message if there is an error determining the file size
    std::perror("Error determining file size");
    return;
  }

  // Print the byte count and the filename
  std::cout << byte_count << " " << filename << std::endl;
}

int main(int argc, char* argv[]) {
  // Check if the number of arguments is correct
  if (argc != 3) {
    // Print the correct usage if the number of arguments is incorrect
    std::cerr << "Usage: " << argv[0] << " -c <filename>" << std::endl;
    return EXIT_FAILURE;
  }

  // Check if the option provided is '-c'
  if (std::string(argv[1]) != "-c") {
    // Print an error message if the option is invalid
    std::cerr << "Error: Invalid option. Only '-c' is supported." << std::endl;
    return EXIT_FAILURE;
  }

  // Call the function to count the bytes in the file
  count_bytes(argv[2]);
  return EXIT_SUCCESS;
}
