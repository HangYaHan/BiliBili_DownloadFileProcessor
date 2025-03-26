// tasks:
// --一个能查找当前目录所有文件的函数，并计数
// --一个能找.m4s文件和.json文件的函数
// --一个解析json文件的函数，找到里面title对应的文本
// --移动前面几个字节的函数
// --MP4转MP3的函数
// --多线程

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#define ORGINAL_FILE_PATH ".\\"

//---debug only---
#define NAME ".\\1209251822"

int show_directory(std::string path);
int store_directory(std::string path, std::vector<std::string> &file_list);
int find_key_file(std::vector<std::string> &file_list, std::string key);

int main()
{
    std::vector<std::string> file_list;
    std::vector<std::string> target_file_list;

    std::cout << "START !\n";
    //show_directory(NAME);
    store_directory(NAME, file_list);
    find_key_file(file_list, ".m4s");
    std::cout << "END !\n";
}

int show_directory(std::string path)
{  
    namespace fs = std::filesystem;

    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path().string() << std::endl;
    }
    return 0;
}

int store_directory(std::string path, std::vector<std::string> &file_list)
{
    namespace fs = std::filesystem;

    for (const auto& entry : fs::directory_iterator(path)) {
        file_list.push_back(entry.path().string());
    }
    return 0;
}

int find_key_file(std::vector<std::string> &file_list, std::string key)
{
    for (auto &file : file_list) {
        if (file.find(key) != std::string::npos) {
            std::cout << "Find key file: " << file << std::endl;
        }
    }
    return 0;
}