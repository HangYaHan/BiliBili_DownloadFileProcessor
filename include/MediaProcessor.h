#pragma once
#include <string>
#include <vector>

class MediaProcessor
{
public:
    // Process m4s files and merge/convert to mp4
    static bool processM4sToMp4(const std::string &m4sFile, const std::wstring &outputName, const std::string &targetPath);
    // Convert mp4 to mp3
    static bool convertMp4ToMp3(const std::string &mp4File, const std::string &mp3File);
    // Determine key m4s file
    static bool judgeKeyM4S(const std::string &m4s1, const std::string &m4s2, std::string &key);

private:
    static constexpr std::string file_suffix_list[] = {"30280"};
};
