#include <iostream>    // For standard input and output
#include <fstream>     // For file handling
#include <cstdio>      // For perror function
#include <cstdlib>     // For EXIT_FAILURE and EXIT_SUCCESS macros
#include <sstream>     // For std::istringstream
#include <locale>      // For locale support
#include <cwchar>      // For wide character functions

// Function to count the number of bytes in a file
void count_bytes(const char* filename, std::size_t& byte_count) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::perror("Error opening file");
        return;
    }

    byte_count = file.tellg();
    file.close();

    if (byte_count == static_cast<std::size_t>(-1)) {
        std::perror("Error determining file size");
        return;
    }
}

// Function to count the number of lines in a file
void count_lines(const char* filename, std::size_t& line_count) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::perror("Error opening file");
        return;
    }

    std::string line;
    line_count = 0;
    while (std::getline(file, line)) {
        ++line_count;
    }
    file.close();
}

// Function to count the number of words in a file
void count_words(const char* filename, std::size_t& word_count) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::perror("Error opening file");
        return;
    }

    std::string line;
    word_count = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            ++word_count;
        }
    }
    file.close();
}

// Function to count the number of characters in a file, considering multibyte characters
void count_characters(const char* filename, std::size_t& char_count) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::perror("Error opening file");
        return;
    }

    std::setlocale(LC_ALL, "");

    std::string line;
    char_count = 0;
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
    file.close();
}

// Function to count lines, words, and bytes in a file
void count_all(const char* filename) {
    std::size_t line_count = 0, word_count = 0, byte_count = 0;
    
    count_lines(filename, line_count);
    count_words(filename, word_count);
    count_bytes(filename, byte_count);

    std::cout << line_count << " " << word_count << " " << byte_count << " " << filename << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [-c|-l|-w|-m] <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string option;
    const char* filename;
    
    if (argc == 2) {
        option = "";
        filename = argv[1];
    } else {
        option = argv[1];
        filename = argv[2];
    }

    if (option == "-c") {
        std::size_t byte_count = 0;
        count_bytes(filename, byte_count);
        std::cout << byte_count << " " << filename << std::endl;
    } else if (option == "-l") {
        std::size_t line_count = 0;
        count_lines(filename, line_count);
        std::cout << line_count << " " << filename << std::endl;
    } else if (option == "-w") {
        std::size_t word_count = 0;
        count_words(filename, word_count);
        std::cout << word_count << " " << filename << std::endl;
    } else if (option == "-m") {
        std::size_t char_count = 0;
        count_characters(filename, char_count);
        std::cout << char_count << " " << filename << std::endl;
    } else if (option == "") {
        count_all(filename);
    } else {
        std::cerr << "Error: Invalid option. Only '-c', '-l', '-w', and '-m' are supported." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
