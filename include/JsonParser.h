#pragma once
#include <string>

class JsonParser
{
public:
    // Parse a JSON file and get the value for the specified key (supports Chinese characters)
    // Requirement: The file must be UTF-8 encoded, and the value can contain Chinese characters
    static bool getValueByKey(const std::string &filename, const std::string &key, std::string &value);
    static bool convertToWString(const std::string &str, std::wstring &wstr);

private:
    const std::string key = "title";
};
