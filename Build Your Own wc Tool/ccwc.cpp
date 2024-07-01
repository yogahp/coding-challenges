#include <iostream>    // For standard input and output
#include <fstream>     // For file handling
#include <cstdio>      // For perror function
#include <cstdlib>     // For EXIT_FAILURE and EXIT_SUCCESS macros
#include <sstream>     // For std::istringstream
#include <locale>      // For locale support
#include <cwchar>      // For wide character functions
#include <vector>      // For std::vector

// Function to count the number of bytes in a buffer
void count_bytes(const std::vector<char>& buffer, std::size_t& byte_count) {
  byte_count = buffer.size();
}

// Function to count the number of lines in a buffer
void count_lines(const std::vector<char>& buffer, std::size_t& line_count) {
  line_count = 0;
  for (std::size_t i = 0; i < buffer.size(); ++i) {
    if (buffer[i] == '\n') {
      ++line_count;
    }
  }
}

// Function to count the number of words in a buffer
void count_words(const std::vector<char>& buffer, std::size_t& word_count) {
  std::istringstream iss(std::string(buffer.begin(), buffer.end()));
  std::string word;
  word_count = 0;
  while (iss >> word) {
    ++word_count;
  }
}

// Function to count the number of characters in a buffer, considering multibyte characters
void count_characters(const std::vector<char>& buffer, std::size_t& char_count) {
  std::setlocale(LC_ALL, "");
  std::mbstate_t state = std::mbstate_t();
  const char* str = buffer.data();
  const char* end = str + buffer.size();
  char_count = 0;

  while (str < end) {
    std::size_t len = std::mbrlen(str, end - str, &state);
    if (len == (std::size_t)-1 || len == (std::size_t)-2) {
      std::perror("Error processing multibyte character");
      return;
    }
    str += len;
    ++char_count;
  }
}

// Function to count lines, words, and bytes in a buffer
void count_all(const std::vector<char>& buffer, const std::string& filename) {
  std::size_t line_count = 0, word_count = 0, byte_count = 0;

  count_lines(buffer, line_count);
  count_words(buffer, word_count);
  count_bytes(buffer, byte_count);

  std::cout << line_count << "  " << word_count << "  " << byte_count;
  if (!filename.empty()) {
    std::cout << "  " << filename;
  }
  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  std::istream* input_stream = &std::cin;
  std::ifstream file;
  std::string filename;
  std::string option;
  std::vector<char> buffer;

  if (argc == 2 && argv[1][0] != '-') {
    // Only filename provided
    filename = argv[1];
    file.open(filename, std::ios::binary);
    if (!file.is_open()) {
      std::perror("Error opening file");
      return EXIT_FAILURE;
    }
    input_stream = &file;
  } else if (argc == 3 && argv[1][0] == '-') {
    // Option and filename provided
    option = argv[1];
    filename = argv[2];
    file.open(filename, std::ios::binary);
    if (!file.is_open()) {
      std::perror("Error opening file");
      return EXIT_FAILURE;
    }
    input_stream = &file;
  } else if (argc == 2 && argv[1][0] == '-') {
    // Only option provided, read from stdin
    option = argv[1];
  } else if (argc > 3) {
    // Incorrect usage
    std::cerr << "Usage: " << argv[0] << " [-c|-l|-w|-m] [<filename>]" << std::endl;
    return EXIT_FAILURE;
  }

  // Read the entire input into a buffer
  buffer.assign(std::istreambuf_iterator<char>(*input_stream), std::istreambuf_iterator<char>());

  if (option == "-c") {
    std::size_t byte_count = 0;
    count_bytes(buffer, byte_count); // Count bytes
    std::cout << byte_count << (filename.empty() ? "" : "  " + filename) << std::endl;
  } else if (option == "-l") {
    std::size_t line_count = 0;
    count_lines(buffer, line_count); // Count lines
    std::cout << line_count << (filename.empty() ? "" : "  " + filename) << std::endl;
  } else if (option == "-w") {
    std::size_t word_count = 0;
    count_words(buffer, word_count); // Count words
    std::cout << word_count << (filename.empty() ? "" : "  " + filename) << std::endl;
  } else if (option == "-m") {
    std::size_t char_count = 0;
    count_characters(buffer, char_count); // Count characters
    std::cout << char_count << (filename.empty() ? "" : "  " + filename) << std::endl;
  } else if (argc == 1 || (argc == 2 && !filename.empty())) {
    count_all(buffer, filename); // Count lines, words, and bytes
  } else {
    std::cerr << "Error: Invalid option. Only '-c', '-l', '-w', and '-m' are supported." << std::endl;
    return EXIT_FAILURE;
  }

  if (file.is_open()) {
    file.close(); // Close the file if it is open
  }

  return EXIT_SUCCESS;
}
