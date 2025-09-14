#pragma once
#include <string>

class JsonParser {
public:
    // 解析json文件，获取指定key的value（支持中文）
    // 要求：文件需为UTF-8编码，value可为中文
    static bool getValueByKey(const std::string& filename, const std::string& key, std::string& value);
    
private:
    const std::string key = "title";
};
