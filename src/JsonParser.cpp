#include "JsonParser.h"

#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <windows.h>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

bool JsonParser::getValueByKey(const std::string &filename, const std::string &key, std::string &value)
{
    rapidjson::Document document;
    FILE *fp = fopen(filename.c_str(), "r");
    if (fp == nullptr)
    {
        perror("Failed to open file");
        return false;
    }

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    document.ParseStream(is);

    if (document.HasMember(key.c_str()))
    {
        value = document[key.c_str()].GetString();
    }
    else
    {
        std::cerr << "Key not found or invalid type." << std::endl;
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}

bool JsonParser::convertToWString(const std::string &str, std::wstring &wstr)
{
    int wlen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    if (wlen <= 0)
    {
        wstr.clear();
        return false;
    }
    std::wstring result(wlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], wlen);
    if (!result.empty() && result.back() == L'\0')
        result.pop_back();
    wstr = result;
    return true;
}