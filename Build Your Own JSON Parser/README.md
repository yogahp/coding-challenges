# JSON Parser

This is a simple JSON parser implemented in C++. It reads a JSON file, parses it, and determines whether it is valid JSON or not.

## Features

- Parses JSON objects, arrays, strings, numbers, booleans, and null values.
- Handles nested JSON objects and arrays.
- Skips whitespace and properly handles escape characters in strings.

## Usage

### Requirements

- A C++ compiler (e.g., `g++`)
- C++ Standard Library

### Compilation

To compile the code, use the following command:

```sh
$ g++ -o json_parser json_parser.cpp
```

### Running the Parser

To run the JSON parser, provide the path to a JSON file as an argument:

```sh
$ ./json_parser <file_path>
```

### Example Run

```sh
$ ./json_parser tests/step1/valid.json  
Valid JSON
```