#include <iostream>
#include <fstream>
#include <string>
#include <regex>

bool isValidJson(const std::string& fileContent) {
  // Remove leading and trailing whitespace
  std::string trimmedContent = fileContent;
  trimmedContent.erase(0, trimmedContent.find_first_not_of(" \t\n\r\f\v"));
  trimmedContent.erase(trimmedContent.find_last_not_of(" \t\n\r\f\v") + 1);

  // Regex to match a valid JSON object with string keys and various value types
  std::regex jsonRegex("\\{\\s*(\"[^\"]+\"\\s*:\\s*(true|false|null|\"[^\"]*\"|[-+]?\\d+(\\.\\d+)?))"
                       "(\\s*,\\s*\"[^\"]+\"\\s*:\\s*(true|false|null|\"[^\"]*\"|[-+]?\\d+(\\.\\d+)?))*\\s*\\}");

  return std::regex_match(trimmedContent, jsonRegex);
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
