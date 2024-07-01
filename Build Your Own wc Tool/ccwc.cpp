#include <iostream>    // For standard input and output
#include <fstream>     // For file handling
#include <cstdio>      // For perror function
#include <cstdlib>     // For EXIT_FAILURE and EXIT_SUCCESS macros
#include <sstream>     // For std::istringstream
#include <locale>      // For locale support
#include <cwchar>      // For wide character functions

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

// Function to count the number of lines in a file
void count_lines(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Print an error message if the file cannot be opened
        std::perror("Error opening file");
        return;
    }

    // Count the number of lines
    std::string line;
    std::size_t line_count = 0;
    while (std::getline(file, line)) {
        ++line_count;
    }
    file.close();  // Close the file

    // Print the line count and the filename
    std::cout << line_count << " " << filename << std::endl;
}

// Function to count the number of words in a file
void count_words(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Print an error message if the file cannot be opened
        std::perror("Error opening file");
        return;
    }

    // Count the number of words
    std::string line;
    std::size_t word_count = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            ++word_count;
        }
    }
    file.close();  // Close the file

    // Print the word count and the filename
    std::cout << word_count << " " << filename << std::endl;
}

// Function to count the number of characters in a file, considering multibyte characters
void count_characters(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Print an error message if the file cannot be opened
        std::perror("Error opening file");
        return;
    }

    // Set the locale to the user's default locale
    std::setlocale(LC_ALL, "");

    // Count the number of characters
    std::string line;
    std::size_t char_count = 0;
    while (std::getline(file, line)) {
        const char* str = line.c_str();
        std::mbstate_t state = std::mbstate_t();
        const char* end = str + line.size();
        while (str < end) {
            std::size_t len = std::mbrlen(str, end - str, &state);
            if (len == (std::size_t)-1 || len == (std::size_t)-2) {
                std::perror("Error processing multibyte character");
                return;
            }
            str += len;
            ++char_count;
        }
        // Add one for the newline character that getline removes
        ++char_count;
    }
    file.close();  // Close the file

    // Print the character count and the filename
    std::cout << char_count << " " << filename << std::endl;
}

int main(int argc, char* argv[]) {
    // Check if the number of arguments is correct
    if (argc != 3) {
        // Print the correct usage if the number of arguments is incorrect
        std::cerr << "Usage: " << argv[0] << " -c|-l|-w|-m <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    // Check if the option provided is '-c', '-l', '-w', or '-m'
    std::string option = argv[1];
    if (option == "-c") {
        // Call the function to count the bytes in the file
        count_bytes(argv[2]);
    } else if (option == "-l") {
        // Call the function to count the lines in the file
        count_lines(argv[2]);
    } else if (option == "-w") {
        // Call the function to count the words in the file
        count_words(argv[2]);
    } else if (option == "-m") {
        // Call the function to count the characters in the file
        count_characters(argv[2]);
    } else {
        // Print an error message if the option is invalid
        std::cerr << "Error: Invalid option. Only '-c', '-l', '-w', and '-m' are supported." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
