
#include "FileManager.h"
#include "JsonParser.h"
#include "MediaProcessor.h"
#include "ThreadUtils.h"
#include "Config.h"


#include <iostream>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <fcntl.h>
#include <io.h>


#define DEBUG 1
#define TEST 0
#define OUTPUT_PATH ".\\Result\\"

#if TEST

int main() {
    // 设置控制台为UTF-8模式，支持wcout输出中文
    _setmode(_fileno(stdout), _O_U8TEXT);
    JsonParser parser;
    std::string test = ".\\1595045647\\videoinfo.json";
    std::string value;
    if (parser.getValueByKey(test, "title", value)) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
        std::wstring wvalue = conv.from_bytes(value);
        std::wcout << L"Title: " << wvalue << std::endl;
    } else {
        std::wcerr << L"Failed to get title from JSON." << std::endl;
    }
    getchar();
    return 0;
}

#else

int main() {
    // 设置控制台为UTF-8模式，支持wcout输出中文
    _setmode(_fileno(stdout), _O_U8TEXT);

    std::vector<std::string> folder_list = FileManager::listDirectory(Config::ORGINAL_FILE_PATH);
    // 只保留数字命名的文件夹
    std::vector<std::string> numeric_folders;
    for (const auto& folder : folder_list) {
        if (FileManager::isNumericDirectory(folder)) {
            numeric_folders.push_back(folder);
        }
    }

#if DEBUG
    std::wcout << L"Numeric folders found:" << std::endl;
    for (const auto& folder : numeric_folders) {
        std::wstring wfolder = std::filesystem::path(folder).wstring();
        std::wcout << wfolder << std::endl;
    }
#endif

    for (const auto& folder : numeric_folders) {
        std::wcout << L"-----------------------------" << std::endl;
        std::wstring wfolder = std::filesystem::path(folder).wstring();
        std::wcout << L"Folder: " << wfolder << std::endl;
        std::vector<std::string> file_list = FileManager::listDirectory(folder);
        auto m4s_files = FileManager::findFilesWithExtension(file_list, ".m4s");
        auto json_files = FileManager::findFilesWithExtension(file_list, ".json");
        if (m4s_files.size() < 2) {
            std::wcerr << L"Not enough .m4s files in " << wfolder << std::endl;
            continue;
        }
        if (json_files.empty()) {
            std::wcerr << L"No .json file in " << wfolder << std::endl;
            continue;
        }
        std::string judged_m4s_file;
        if (!MediaProcessor::judgeKeyMp4(m4s_files[0], m4s_files[1], judged_m4s_file)) {
            continue;
        }

        // 读取json文件中的title（假设key为"title"）
        std::string title_utf8;
        if (!JsonParser::getValueByKey(json_files[0], "title", title_utf8)) {
            std::wcerr << L"Failed to get title from json: " << std::filesystem::path(json_files[0]).wstring() << std::endl;
            continue;
        }
    // utf8转wstring（Windows API）
    int wlen = MultiByteToWideChar(CP_UTF8, 0, title_utf8.c_str(), -1, NULL, 0);
    std::wstring wtitle(wlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, title_utf8.c_str(), -1, &wtitle[0], wlen);
    // 去除末尾多余的\0
    if (!wtitle.empty() && wtitle.back() == L'\0') wtitle.pop_back();

#if DEBUG
        std::wcout << L"Chosen m4s file: " << std::filesystem::path(judged_m4s_file).wstring() << std::endl;
        std::wcout << L"Title from json: " << wtitle << std::endl;
#endif

        // 用json中的title作为输出文件名，输出到 OUTPUT_PATH 下
        MediaProcessor::processM4sToMp4(judged_m4s_file, title_utf8, OUTPUT_PATH);
        std::wcout << L"-----------------------------" << std::endl;
    }
    return 0;
}

#endif