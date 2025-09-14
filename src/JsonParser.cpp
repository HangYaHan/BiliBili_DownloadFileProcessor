#include "JsonParser.h"
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>

bool JsonParser::getValueByKey(const std::string& filename, const std::string& key, std::string& value) {
    // 以UTF-8模式读取
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    size_t keyPos = jsonContent.find("\"" + key + "\"");
    if (keyPos == std::string::npos) {
        std::cerr << "Failed to find key: " << key << std::endl;
        return false;
    }
    size_t colonPos = jsonContent.find(":", keyPos);
    if (colonPos == std::string::npos) {
        std::cerr << "Failed to find colon after key: " << key << std::endl;
        return false;
    }
    size_t valueStart = jsonContent.find("\"", colonPos + 1);
    size_t valueEnd = jsonContent.find("\"", valueStart + 1);
    if (valueStart == std::string::npos || valueEnd == std::string::npos) {
        std::cerr << "Failed to find value for key: " << key << std::endl;
        return false;
    }
    value = jsonContent.substr(valueStart + 1, valueEnd - valueStart - 1);
    return true;
}
