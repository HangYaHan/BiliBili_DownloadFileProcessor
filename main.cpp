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
#define TARGET_FILE_PATH ".\\target\\"

//---debug only---
#define NAME ".\\1209251822"

bool show_directory(std::string path);
bool store_directory(std::string path, std::vector<std::string> &file_list);
bool show_key_file(std::vector<std::string> &file_list, std::string key);
bool store_key_file(std::vector<std::string> &file_list, std::string key, std::string &target_file);
bool store_key_file(std::vector<std::string> &file_list, std::string key, std::string &target_file1, std::string &target_file2);
//bool find_key_value_in_json(const std::string& filename, const std::string& key, std::string& name);
bool process_file(const std::string& filename, const std::string& newfilename, const std::string& target_path);
bool judge_key_mp4(std::string &key, std::string &m4s1, std::string &m4s2);

int main()
{
    std::vector<std::string> file_list;
    std::string target_directory = NAME;
    std::string target_json_file;
    std::string target_m4s_file1;
    std::string target_m4s_file2;
    std::string judged_m4s_file; 
    std::string name;

    store_directory(target_directory, file_list);
    //---
    std::cout << "Start !" << std::endl;
    store_key_file(file_list, ".m4s", target_m4s_file1, target_m4s_file2);
    store_key_file(file_list, ".json", target_json_file);
    judge_key_mp4(judged_m4s_file, target_m4s_file1, target_m4s_file2);
    std::cout << "Judged file is " << judged_m4s_file << std::endl;
    name = "test";
    process_file(judged_m4s_file, name, TARGET_FILE_PATH);
}

bool show_directory(std::string path)
{  
    namespace fs = std::filesystem;

    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path().string() << std::endl;
    }
    return 0;
}

bool store_directory(std::string path, std::vector<std::string> &file_list)
{
    namespace fs = std::filesystem;

    for (const auto& entry : fs::directory_iterator(path)) {
        file_list.push_back(entry.path().string());
    }
    return 0;
}

bool show_key_file(std::vector<std::string> &file_list, std::string key)
{
    for (auto &file : file_list) {
        if (file.find(key) != std::string::npos) {
            std::cout << "Find key file: " << file << std::endl;
        }
    }
    return 0;
}

bool store_key_file(std::vector<std::string> &file_list, std::string key, std::string &target_file)
{
    for (auto &file : file_list) {
        if (file.find(key) != std::string::npos) {
            target_file = file;
            std::cout << "Find " << key << " file : " << file << std::endl;
        }
    }
    return 0;
}

bool store_key_file(std::vector<std::string> &file_list, std::string key, std::string &target_file1, std::string &target_file2)
{
    bool foundFirst = false;
    for (auto &file : file_list) {
        if (file.find(key) != std::string::npos) {
            if (!foundFirst) {
                target_file1 = file;
                std::cout << "Find " << key << " file1 : " << file << std::endl;
                foundFirst = true;
            } else {
                target_file2 = file;
                std::cout << "Find " << key << " file2 : " << file << std::endl;
                break;
            }
        }
    }
    return 0;
}

bool find_key_value_in_json(const std::string& filename, const std::string& key, std::string& name) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string jsonContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

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

    name =  jsonContent.substr(valueStart + 1, valueEnd - valueStart - 1);

    return true;
}

bool process_file(const std::string& filename, const std::string& name, const std::string& target_path) {
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string newFilename = target_path + name + ".mp4";

    std::ofstream outputFile(newFilename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error creating output file: " << newFilename << std::endl;
        return false;
    }

    inputFile.seekg(9, std::ios::beg);

    char buffer[1024];
    while (inputFile.read(buffer, sizeof(buffer))) {
        outputFile.write(buffer, inputFile.gcount());
    }
    outputFile.write(buffer, inputFile.gcount());

    std::cout << "File processed successfully: " << newFilename << std::endl;
    return true;
}

bool judge_key_mp4(std::string &key, std::string &m4s1, std::string &m4s2)
{
    key = "111";
    std::ifstream file(m4s1, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << m4s1 << std::endl;
        return false;
    }

    file.seekg(130, std::ios::beg); // Move to the 28th byte (0-based index)
    char byteData;
    file.read(&byteData, 1);

    if (file.gcount() == 1) {
        std::cout << "The 28th byte of the " << m4s1 << " is: " << static_cast<int>(byteData) << std::endl;
        if (byteData == 0x6F) {
            key = m4s2;
            return true;
        }
        else {
            key = m4s1;
            return true;
        }
        return true;
    } else {
        std::cerr << "Failed to read the 28th byte of the file." << std::endl;
        return false;
    }
    return false;
}