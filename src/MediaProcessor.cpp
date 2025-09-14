#include "MediaProcessor.h"
#include <fstream>
#include <iostream>

bool MediaProcessor::processM4sToMp4(const std::string& m4sFile, const std::string& outputName, const std::string& targetPath) {
    std::ifstream inputFile(m4sFile, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << m4sFile << std::endl;
        return false;
    }
    std::string newFilename = targetPath + outputName + ".mp4";
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

bool MediaProcessor::convertMp4ToMp3(const std::string& mp4File, const std::string& mp3File) {
    // 这里建议用ffmpeg等外部库实现，暂留空实现
    std::cerr << "convertMp4ToMp3 not implemented. Please use ffmpeg or similar tool." << std::endl;
    return false;
}

bool MediaProcessor::judgeKeyMp4(const std::string& m4s1, const std::string& m4s2, std::string& key) {
    if (m4s1.find("30280") != std::string::npos) {
        key = m4s1;
    } else if (m4s2.find("30280") != std::string::npos) {
        key = m4s2;
    } else {
        std::cerr << "Neither file contains '30280' in the name." << std::endl;
        return false;
    }
    return true;
}
