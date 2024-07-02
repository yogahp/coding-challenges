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

// Function to check if the given string is a valid JSON
bool isValidJson(const std::string& input) {
  size_t pos = 0; // Initialize position to start of the string
  skipWhitespace(input, pos); // Skip any leading whitespace
  
  // Try to parse as either an object or array
  if (!parseObject(input, pos) && !parseArray(input, pos)) {
    return false; // If neither parsing succeeds, return false
  }
  
  skipWhitespace(input, pos); // Skip any trailing whitespace
  return pos == input.length(); // Check if entire string has been parsed
}

// Function to parse any JSON value
bool parseValue(const std::string& input, size_t& pos) {
  skipWhitespace(input, pos); // Skip leading whitespace
  if (pos >= input.length()) return false; // If position is out of bounds, return false

  switch (input[pos]) { // Check the current character to determine the type of value
    case '{':
      return parseObject(input, pos); // Parse as object
    case '[':
      return parseArray(input, pos); // Parse as array
    case '"':
      return parseString(input, pos); // Parse as string
    case 't': case 'f':
      return parseBoolean(input, pos); // Parse as boolean
    case 'n':
      return parseNull(input, pos); // Parse as null
    default:
      return parseNumber(input, pos); // Parse as number
  }
}

// Function to parse a JSON object
bool parseObject(const std::string& input, size_t& pos) {
  if (input[pos] != '{') return false; // Check for opening brace
  pos++; // Move past '{'
  skipWhitespace(input, pos); // Skip whitespace

  if (input[pos] == '}') { // Check for empty object
    pos++; // Move past '}'
    return true;
  }

  while (true) {
    if (!parseString(input, pos)) return false; // Parse key as string
    skipWhitespace(input, pos); // Skip whitespace
    if (input[pos] != ':') return false; // Check for colon
    pos++; // Move past ':'
    if (!parseValue(input, pos)) return false; // Parse value
    skipWhitespace(input, pos); // Skip whitespace
    if (input[pos] == '}') { // Check for closing brace
      pos++; // Move past '}'
      return true;
    }
    if (input[pos] != ',') return false; // Check for comma between key-value pairs
    pos++; // Move past ','
    skipWhitespace(input, pos); // Skip whitespace
  }
}

// Function to parse a JSON array
bool parseArray(const std::string& input, size_t& pos) {
  if (input[pos] != '[') return false; // Check for opening bracket
  pos++; // Move past '['
  skipWhitespace(input, pos); // Skip whitespace

  if (input[pos] == ']') { // Check for empty array
    pos++; // Move past ']'
    return true;
  }

  while (true) {
    if (!parseValue(input, pos)) return false; // Parse value
    skipWhitespace(input, pos); // Skip whitespace
    if (input[pos] == ']') { // Check for closing bracket
      pos++; // Move past ']'
      return true;
    }
    if (input[pos] != ',') return false; // Check for comma between values
    pos++; // Move past ','
    skipWhitespace(input, pos); // Skip whitespace
  }
}

// Function to parse a JSON string
bool parseString(const std::string& input, size_t& pos) {
  if (input[pos] != '"') return false; // Check for opening quote
  pos++; // Move past '"'
  while (pos < input.length() && input[pos] != '"') { // Loop until closing quote or end of string
    if (input[pos] == '\\') { // Check for escape character
      pos++; // Skip escaped character
    }
    pos++; // Move to next character
  }
  if (pos >= input.length() || input[pos] != '"') return false; // Check for closing quote
  pos++; // Move past closing quote
  return true;
}

// Function to parse a JSON number
bool parseNumber(const std::string& input, size_t& pos) {
  size_t start = pos; // Record start position
  if (input[pos] == '-') pos++; // Handle optional negative sign
  if (isdigit(input[pos])) { // Check if digit
    if (input[pos] == '0') {
      pos++; // Handle leading zero
    } else {
      while (isdigit(input[pos])) pos++; // Parse integer part
    }
  } else {
    return false;
  }
  if (input[pos] == '.') { // Handle decimal part
    pos++;
    if (!isdigit(input[pos])) return false;
    while (isdigit(input[pos])) pos++;
  }
  if (input[pos] == 'e' || input[pos] == 'E') { // Handle exponent part
    pos++;
    if (input[pos] == '+' || input[pos] == '-') pos++;
    if (!isdigit(input[pos])) return false;
    while (isdigit(input[pos])) pos++;
  }
  return pos > start; // Ensure at least one digit was parsed
}

// Function to parse a JSON boolean
bool parseBoolean(const std::string& input, size_t& pos) {
  if (input.substr(pos, 4) == "true") { // Check for "true"
    pos += 4;
    return true;
  }
  if (input.substr(pos, 5) == "false") { // Check for "false"
    pos += 5;
    return true;
  }
  return false;
}

// Function to parse a JSON null
bool parseNull(const std::string& input, size_t& pos) {
  if (input.substr(pos, 4) == "null") { // Check for "null"
    pos += 4;
    return true;
  }
  return false;
}

// Function to skip whitespace characters
void skipWhitespace(const std::string& input, size_t& pos) {
  while (pos < input.length() && isspace(input[pos])) { // Loop through whitespace characters
    pos++;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) { // Check for correct number of arguments
    std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
    return 1;
  }

  std::string filePath = argv[1]; // Get file path from arguments
  std::ifstream file(filePath); // Open file
  if (!file) { // Check if file opened successfully
    std::cerr << "File not found" << std::endl;
    return 1;
  }

  std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); // Read file content

  if (isValidJson(fileContent)) { // Check if file content is valid JSON
    std::cout << "Valid JSON" << std::endl;
    return 0;
  } else {
    std::cerr << "Invalid JSON" << std::endl;
    return 1;
  }
}
