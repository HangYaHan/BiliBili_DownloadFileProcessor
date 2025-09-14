#pragma once
#include <string>
#include <vector>

class FileManager {
public:
    // 遍历目录，返回所有文件/文件夹路径
    static std::vector<std::string> listDirectory(const std::string& path);
    // 查找指定后缀的文件
    static std::vector<std::string> findFilesWithExtension(const std::vector<std::string>& fileList, const std::string& ext);
    // 判断是否为数字命名的文件夹
    static bool isNumericDirectory(const std::string& path);
};
