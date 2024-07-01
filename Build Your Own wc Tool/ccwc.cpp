#include <iostream>    // For standard input and output
#include <fstream>     // For file handling
#include <cstdio>      // For perror function
#include <cstdlib>     // For EXIT_FAILURE and EXIT_SUCCESS macros
#include <sstream>     // For std::istringstream
#include <locale>      // For locale support
#include <cwchar>      // For wide character functions

// Function to count the number of bytes in a file or standard input
void count_bytes(std::istream& stream, std::size_t& byte_count) {
    std::streampos initial_pos = stream.tellg(); // Save the initial position
    stream.seekg(0, std::ios::end);              // Move to the end of the stream
    byte_count = stream.tellg();                 // Get the current position, which is the size of the stream
    stream.seekg(initial_pos);                   // Restore the initial position
}

// Function to count the number of lines in a file or standard input
void count_lines(std::istream& stream, std::size_t& line_count) {
    std::string line;
    line_count = 0;
    while (std::getline(stream, line)) {         // Read each line from the stream
        ++line_count;                            // Increment the line count
    }
}

// Function to count the number of words in a file or standard input
void count_words(std::istream& stream, std::size_t& word_count) {
    std::string line;
    word_count = 0;
    while (std::getline(stream, line)) {         // Read each line from the stream
        std::istringstream iss(line);            // Create a string stream from the line
        std::string word;
        while (iss >> word) {                    // Read each word from the line
            ++word_count;                        // Increment the word count
        }
    }
}

// Function to count the number of characters in a file or standard input, considering multibyte characters
void count_characters(std::istream& stream, std::size_t& char_count) {
    std::setlocale(LC_ALL, "");                  // Set the locale to the user's default locale

    std::string line;
    char_count = 0;
    while (std::getline(stream, line)) {         // Read each line from the stream
        const char* str = line.c_str();
        std::mbstate_t state = std::mbstate_t();
        const char* end = str + line.size();
        while (str < end) {                      // Process each multibyte character
            std::size_t len = std::mbrlen(str, end - str, &state);
            if (len == (std::size_t)-1 || len == (std::size_t)-2) {
                std::perror("Error processing multibyte character");
                return;
            }
            str += len;
            ++char_count;                        // Increment the character count
        }
        ++char_count;                            // Add one for the newline character
    }
}

// Function to count lines, words, and bytes in a file or standard input
void count_all(std::istream& stream) {
    std::size_t line_count = 0, word_count = 0, byte_count = 0;

    count_lines(stream, line_count);             // Count lines
    stream.clear();                              // Clear EOF flag to allow further use of the stream
    stream.seekg(0);                             // Reset stream position to the beginning
    count_words(stream, word_count);             // Count words
    stream.clear();                              // Clear EOF flag to allow further use of the stream
    stream.seekg(0);                             // Reset stream position to the beginning
    count_bytes(stream, byte_count);             // Count bytes

    std::cout << line_count << " " << word_count << " " << byte_count << std::endl;
}

int main(int argc, char* argv[]) {
    std::istream* input_stream = &std::cin;      // Default to standard input
    std::ifstream file;
    std::string filename;
    std::string option;

    if (argc == 2 && argv[1][0] != '-') {
        // Only filename provided
        filename = argv[1];
        file.open(filename, std::ios::binary);   // Open the file
        if (!file.is_open()) {
            std::perror("Error opening file");
            return EXIT_FAILURE;
        }
        input_stream = &file;                    // Use the file as input stream
    } else if (argc == 3 && argv[1][0] == '-') {
        // Option and filename provided
        option = argv[1];
        filename = argv[2];
        file.open(filename, std::ios::binary);   // Open the file
        if (!file.is_open()) {
            std::perror("Error opening file");
            return EXIT_FAILURE;
        }
        input_stream = &file;                    // Use the file as input stream
    } else if (argc == 2 && argv[1][0] == '-') {
        // Only option provided, read from stdin
        option = argv[1];
    } else if (argc > 3) {
        // Incorrect usage
        std::cerr << "Usage: " << argv[0] << " [-c|-l|-w|-m] [<filename>]" << std::endl;
        return EXIT_FAILURE;
    }

    if (option == "-c") {
        std::size_t byte_count = 0;
        count_bytes(*input_stream, byte_count);  // Count bytes
        std::cout << byte_count << (filename.empty() ? "" : " " + filename) << std::endl;
    } else if (option == "-l") {
        std::size_t line_count = 0;
        count_lines(*input_stream, line_count);  // Count lines
        std::cout << line_count << (filename.empty() ? "" : " " + filename) << std::endl;
    } else if (option == "-w") {
        std::size_t word_count = 0;
        count_words(*input_stream, word_count);  // Count words
        std::cout << word_count << (filename.empty() ? "" : " " + filename) << std::endl;
    } else if (option == "-m") {
        std::size_t char_count = 0;
        count_characters(*input_stream, char_count); // Count characters
        std::cout << char_count << (filename.empty() ? "" : " " + filename) << std::endl;
    } else if (argc == 1 || (argc == 2 && !filename.empty())) {
        count_all(*input_stream);                // Count lines, words, and bytes
    } else {
        std::cerr << "Error: Invalid option. Only '-c', '-l', '-w', and '-m' are supported." << std::endl;
        return EXIT_FAILURE;
    }

    if (file.is_open()) {
        file.close();                            // Close the file if it is open
    }

    return EXIT_SUCCESS;
}
