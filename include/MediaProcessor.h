#pragma once
#include <string>
#include <vector>

class MediaProcessor {
public:
    // 处理m4s文件，合成/转换为mp4
    static bool processM4sToMp4(const std::string& m4sFile, const std::string& outputName, const std::string& targetPath);
    // mp4转mp3
    static bool convertMp4ToMp3(const std::string& mp4File, const std::string& mp3File);
    // 判断关键m4s文件
    static bool judgeKeyMp4(const std::string& m4s1, const std::string& m4s2, std::string& key);
};
