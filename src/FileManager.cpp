#include "FileManager.h"
#include <filesystem>
#include <algorithm>

std::vector<std::string> FileManager::listDirectory(const std::string& path) {
    namespace fs = std::filesystem;
    std::vector<std::string> result;
    for (const auto& entry : fs::directory_iterator(path)) {
        result.push_back(entry.path().string());
    }
    return result;
}

std::vector<std::string> FileManager::findFilesWithExtension(const std::vector<std::string>& fileList, const std::string& ext) {
    std::vector<std::string> result;
    for (const auto& file : fileList) {
        if (file.size() >= ext.size() && file.substr(file.size() - ext.size()) == ext) {
            result.push_back(file);
        }
    }
    return result;
}

bool FileManager::isNumericDirectory(const std::string& path) {
    namespace fs = std::filesystem;
    std::string folder_name = fs::path(path).filename().string();
    return std::all_of(folder_name.begin(), folder_name.end(), ::isdigit);
}
