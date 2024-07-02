#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

// Function declarations
bool isValidJson(const std::string& input);
bool parseValue(const std::string& input, size_t& pos);
bool parseObject(const std::string& input, size_t& pos);
bool parseArray(const std::string& input, size_t& pos);
bool parseString(const std::string& input, size_t& pos);
bool parseNumber(const std::string& input, size_t& pos);
bool parseBoolean(const std::string& input, size_t& pos);
bool parseNull(const std::string& input, size_t& pos);
void skipWhitespace(const std::string& input, size_t& pos);

bool isValidJson(const std::string& input) {
  size_t pos = 0;
  skipWhitespace(input, pos);
  if (!parseObject(input, pos) && !parseArray(input, pos)) {
    return false;
  }
  skipWhitespace(input, pos);
  return pos == input.length();
}

bool parseValue(const std::string& input, size_t& pos) {
  skipWhitespace(input, pos);
  if (pos >= input.length()) return false;

  switch (input[pos]) {
    case '{':
      return parseObject(input, pos);
    case '[':
      return parseArray(input, pos);
    case '"':
      return parseString(input, pos);
    case 't': case 'f':
      return parseBoolean(input, pos);
    case 'n':
      return parseNull(input, pos);
    default:
      return parseNumber(input, pos);
  }
}

bool parseObject(const std::string& input, size_t& pos) {
  if (input[pos] != '{') return false;
  pos++;
  skipWhitespace(input, pos);

  if (input[pos] == '}') {
    pos++;
    return true;
  }

  while (true) {
    if (!parseString(input, pos)) return false;
    skipWhitespace(input, pos);
    if (input[pos] != ':') return false;
    pos++;
    if (!parseValue(input, pos)) return false;
    skipWhitespace(input, pos);
    if (input[pos] == '}') {
      pos++;
      return true;
    }
    if (input[pos] != ',') return false;
    pos++;
    skipWhitespace(input, pos);
  }
}

bool parseArray(const std::string& input, size_t& pos) {
  if (input[pos] != '[') return false;
  pos++;
  skipWhitespace(input, pos);

  if (input[pos] == ']') {
    pos++;
    return true;
  }

  while (true) {
    if (!parseValue(input, pos)) return false;
    skipWhitespace(input, pos);
    if (input[pos] == ']') {
      pos++;
      return true;
    }
    if (input[pos] != ',') return false;
    pos++;
    skipWhitespace(input, pos);
  }
}

bool parseString(const std::string& input, size_t& pos) {
  if (input[pos] != '"') return false;
  pos++;
  while (pos < input.length() && input[pos] != '"') {
    if (input[pos] == '\\') {
      pos++; // skip escaped character
    }
    pos++;
  }
  if (pos >= input.length() || input[pos] != '"') return false;
  pos++;
  return true;
}

bool parseNumber(const std::string& input, size_t& pos) {
  size_t start = pos;
  if (input[pos] == '-') pos++;
  if (isdigit(input[pos])) {
    if (input[pos] == '0') {
      pos++;
    } else {
      while (isdigit(input[pos])) pos++;
    }
  } else {
    return false;
  }
  if (input[pos] == '.') {
    pos++;
    if (!isdigit(input[pos])) return false;
    while (isdigit(input[pos])) pos++;
  }
  if (input[pos] == 'e' || input[pos] == 'E') {
    pos++;
    if (input[pos] == '+' || input[pos] == '-') pos++;
    if (!isdigit(input[pos])) return false;
    while (isdigit(input[pos])) pos++;
  }
  return pos > start;
}

bool parseBoolean(const std::string& input, size_t& pos) {
  if (input.substr(pos, 4) == "true") {
    pos += 4;
    return true;
  }
  if (input.substr(pos, 5) == "false") {
    pos += 5;
    return true;
  }
  return false;
}

bool parseNull(const std::string& input, size_t& pos) {
  if (input.substr(pos, 4) == "null") {
    pos += 4;
    return true;
  }
  return false;
}

void skipWhitespace(const std::string& input, size_t& pos) {
  while (pos < input.length() && isspace(input[pos])) {
    pos++;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
    return 1;
  }

  std::string filePath = argv[1];
  std::ifstream file(filePath);
  if (!file) {
    std::cerr << "File not found" << std::endl;
    return 1;
  }

  std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  if (isValidJson(fileContent)) {
    std::cout << "Valid JSON" << std::endl;
    return 0;
  } else {
    std::cerr << "Invalid JSON" << std::endl;
    return 1;
  }
}
