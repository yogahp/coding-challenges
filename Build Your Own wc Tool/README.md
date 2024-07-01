# CCWC

`CCWC` is a simple command-line utility for counting characters, words, and lines in a text file or from standard input. It supports multiple options to display different counts.

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