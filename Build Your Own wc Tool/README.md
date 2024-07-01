# CCWC

`CCWC` (Count Characters, Words, and Lines) is a simple command-line utility for counting characters, words, and lines in a text file or from standard input. It supports multiple options to display different counts.

## Features

- Count characters, words, and lines in a file or from standard input.
- Supports counting:
  - Bytes (`-c` option)
  - Lines (`-l` option)
  - Words (`-w` option)
  - Characters (`-m` option)

## Usage

```sh
ccwc [-c|-l|-w|-m] [<filename>]
```

## Compilation

To compile the program, use the following command:

```sh
g++ -o ccwc ccwc.cpp
```

## Examples

### Counting from a file

Count the number of lines in `test.txt`:

```sh
./ccwc -l test.txt
```

Count the number of words in `test.txt`:

```sh
./ccwc -w test.txt
```

Count the number of bytes in `test.txt`:

```sh
./ccwc -c test.txt
```

Count the number of characters in `test.txt`:

```sh
./ccwc -m test.txt
```

### Counting from standard input

You can use `cat` to pipe the contents of a file into `ccwc`:

Count the number of lines:

```sh
cat test.txt | ./ccwc -l
```

Count the number of words:

```sh
cat test.txt | ./ccwc -w
```

Count the number of bytes:

```sh
cat test.txt | ./ccwc -c
```

Count the number of characters:

```sh
cat test.txt | ./ccwc -m
```

### Counting all metrics from a file

If no option is specified, `ccwc` will count lines, words, and bytes:

```sh
./ccwc test.txt
```

### Counting all metrics from standard input

If no option and filename are specified, `ccwc` will read from standard input and count lines, words, and bytes:

```sh
cat test.txt | ./ccwc
```

## Source Code

The source code is available in `ccwc.cpp`. Here is a brief explanation of the code:

### Includes

```cpp
#include <iostream>    // For standard input and output
#include <fstream>     // For file handling
#include <cstdio>      // For perror function
#include <cstdlib>     // For EXIT_FAILURE and EXIT_SUCCESS macros
#include <sstream>     // For std::istringstream
#include <locale>      // For locale support
#include <cwchar>      // For wide character functions
```

### Function `count_bytes`

```cpp
void count_bytes(std::istream& stream, std::size_t& byte_count) {
    std::streampos initial_pos = stream.tellg();
    stream.seekg(0, std::ios::end);
    byte_count = stream.tellg();
    stream.seekg(initial_pos);
}
```

### Function `count_lines`

```cpp
void count_lines(std::istream& stream, std::size_t& line_count) {
    std::string line;
    line_count = 0;
    while (std::getline(stream, line)) {
        ++line_count;
    }
}
```

### Function `count_words`

```cpp
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
}
```

### Function `count_characters`

```cpp
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
        ++char_count;
    }
}
```

### Function `count_all`

```cpp
void count_all(std::istream& stream) {
    std::size_t line_count = 0, word_count = 0, byte_count = 0;

    count_lines(stream, line_count);
    stream.clear(); // Clear EOF flag to allow further use of the stream
    stream.seekg(0); // Reset stream position to the beginning
    count_words(stream, word_count);
    stream.clear(); // Clear EOF flag to allow further use of the stream
    stream.seekg(0); // Reset stream position to the beginning
    count_bytes(stream, byte_count);

    std::cout << line_count << " " << word_count << " " << byte_count << std::endl;
}
```

### `main` Function

```cpp
int main(int argc, char* argv[]) {
    std::istream* input_stream = &std::cin;
    std::ifstream file;
    std::string filename;
    std::string option;

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

    if (option == "-c") {
        std::size_t byte_count = 0;
        count_bytes(*input_stream, byte_count);
        std::cout << byte_count << (filename.empty() ? "" : " " + filename) << std::endl;
    } else if (option == "-l") {
        std::size_t line_count = 0;
        count_lines(*input_stream, line_count);
        std::cout << line_count << (filename.empty() ? "" : " " + filename) << std::endl;
    } else if (option == "-w") {
        std::size_t word_count = 0;
        count_words(*input_stream, word_count);
        std::cout << word_count << (filename.empty() ? "" : " " + filename) << std::endl;
    } else if (option == "-m") {
        std::size_t char_count = 0;
        count_characters(*input_stream, char_count);
        std::cout << char_count << (filename.empty() ? "" : " " + filename) << std::endl;
    } else if (argc == 1 || (argc == 2 && !filename.empty())) {
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
```