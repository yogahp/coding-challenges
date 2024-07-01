#include <iostream>    // For standard input and output
#include <fstream>     // For file handling
#include <cstdio>      // For perror function
#include <cstdlib>     // For EXIT_FAILURE and EXIT_SUCCESS macros
#include <sstream>     // For std::istringstream
#include <locale>      // For locale support
#include <cwchar>      // For wide character functions

// Function to count the number of bytes in a file or standard input
void count_bytes(std::istream& stream, std::size_t& byte_count) {
    std::streampos initial_pos = stream.tellg();
    stream.seekg(0, std::ios::end);
    byte_count = stream.tellg();
    stream.seekg(initial_pos);
}

// Function to count the number of lines in a file or standard input
void count_lines(std::istream& stream, std::size_t& line_count) {
    std::string line;
    line_count = 0;
    while (std::getline(stream, line)) {
        ++line_count;
    }
    stream.clear(); // Clear EOF flag to allow further use of the stream
    stream.seekg(0); // Reset stream position to the beginning
}

// Function to count the number of words in a file or standard input
void count_words(std::istream& stream, std::size_t& word_count) {
    std::string line;
    word_count = 0;
    while (std::getline(stream, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            ++word_count;
        }
    }
    stream.clear(); // Clear EOF flag to allow further use of the stream
    stream.seekg(0); // Reset stream position to the beginning
}

// Function to count the number of characters in a file or standard input, considering multibyte characters
void count_characters(std::istream& stream, std::size_t& char_count) {
    std::setlocale(LC_ALL, "");

    std::string line;
    char_count = 0;
    while (std::getline(stream, line)) {
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
    stream.clear(); // Clear EOF flag to allow further use of the stream
    stream.seekg(0); // Reset stream position to the beginning
}

// Function to count lines, words, and bytes in a file or standard input
void count_all(std::istream& stream) {
    std::size_t line_count = 0, word_count = 0, byte_count = 0;

    count_lines(stream, line_count);
    count_words(stream, word_count);
    count_bytes(stream, byte_count);

    std::cout << line_count << " " << word_count << " " << byte_count << std::endl;
}

int main(int argc, char* argv[]) {
    std::istream* input_stream = &std::cin;
    std::ifstream file;

    if (argc == 3) {
        file.open(argv[2], std::ios::binary);
        if (!file.is_open()) {
            std::perror("Error opening file");
            return EXIT_FAILURE;
        }
        input_stream = &file;
    } else if (argc != 1 && argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [-c|-l|-w|-m] [<filename>]" << std::endl;
        return EXIT_FAILURE;
    }

    std::string option;
    if (argc == 2) {
        option = argv[1];
    }

    if (option == "-c") {
        std::size_t byte_count = 0;
        count_bytes(*input_stream, byte_count);
        std::cout << byte_count << std::endl;
    } else if (option == "-l") {
        std::size_t line_count = 0;
        count_lines(*input_stream, line_count);
        std::cout << line_count << std::endl;
    } else if (option == "-w") {
        std::size_t word_count = 0;
        count_words(*input_stream, word_count);
        std::cout << word_count << std::endl;
    } else if (option == "-m") {
        std::size_t char_count = 0;
        count_characters(*input_stream, char_count);
        std::cout << char_count << std::endl;
    } else if (argc == 1) {
        count_all(*input_stream);
    } else {
        std::cerr << "Error: Invalid option. Only '-c', '-l', '-w', and '-m' are supported." << std::endl;
        return EXIT_FAILURE;
    }

    if (file.is_open()) {
        file.close();
    }

    return EXIT_SUCCESS;
}
