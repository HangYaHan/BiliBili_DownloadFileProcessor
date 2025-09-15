#include "MediaProcessor.h"
#include <fstream>
#include <iostream>
#include <windows.h>
#include <filesystem>

bool MediaProcessor::processM4sToMp4(const std::string &m4sFile, const std::wstring &outputName, const std::string &targetPath)
{
    std::ifstream inputFile(m4sFile, std::ios::binary);
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file: " << m4sFile << std::endl;
        return false;
    }
    // Check and fix path separators
    std::string fixedTargetPath = targetPath;
    if (!fixedTargetPath.empty() && fixedTargetPath.back() != '\\' && fixedTargetPath.back() != '/')
    {
        fixedTargetPath += "\\";
    }
    // deal with illegal characters in outputName
    std::wstring safeName = outputName;
    for (auto &ch : safeName)
    {
        if (ch == L'/' || ch == L'\\' || ch == L':' || ch == L'*' || ch == L'?' || ch == L'"' || ch == L'<' || ch == L'>' || ch == L'|')
        {
            ch = L'_';
        }
    }
    // create output directory if it doesn't exist
    try
    {
        std::filesystem::create_directories(fixedTargetPath);
    }
    catch (...)
    {
        std::wcerr << L"Failed to create output directory: " << std::wstring(fixedTargetPath.begin(), fixedTargetPath.end()) << std::endl;
        return false;
    }

    // using std::filesystem to construct paths and filenames
    std::filesystem::path dir(targetPath);
    std::filesystem::create_directories(dir);
    std::filesystem::path file = dir / (safeName + L".mp4");

    // wfopen for binary write, chinese characters supported
    FILE *fp = _wfopen(file.native().c_str(), L"wb");
    if (!fp)
    {
        std::wcerr << L"Error creating output file: " << file.native() << std::endl;
        return false;
    }
    inputFile.seekg(9, std::ios::beg);
    char buffer[1024];
    while (inputFile.read(buffer, sizeof(buffer)))
    {
        fwrite(buffer, 1, inputFile.gcount(), fp);
    }
    fwrite(buffer, 1, inputFile.gcount(), fp);
    fclose(fp);
    return true;
}

bool MediaProcessor::judgeKeyM4S(const std::string &m4s1, const std::string &m4s2, std::string &key)
{
    for (const auto &suffix : file_suffix_list)
    {
        if (m4s1.find(suffix) != std::string::npos)
        {
            key = m4s1;
            return true;
        }
        if (m4s2.find(suffix) != std::string::npos)
        {
            key = m4s2;
            return true;
        }
    }
    std::cerr << "Neither file contains any of the specified suffixes in the name." << std::endl;
    return false;
}
