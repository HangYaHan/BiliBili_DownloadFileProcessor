#include "FileManager.h"
#include "JsonParser.h"
#include "MediaProcessor.h"
#include "Config.h"

#include <iostream>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

#define OUTPUT_PATH ".\\Result\\"
#define KEY_TITLE "title"

int main()
{
    // Set console to handle UTF-16 output
    // So Chinese characters can be displayed correctly
    _setmode(_fileno(stdout), _O_U16TEXT);

    std::vector<std::string> folder_list = FileManager::listDirectory(Config::ORGINAL_FILE_PATH);
    // Preserve only numeric-named folders
    std::vector<std::string> numeric_folders;
    for (const auto &folder : folder_list)
    {
        if (FileManager::isNumericDirectory(folder))
        {
            numeric_folders.push_back(folder);
        }
    }

    // #if DEBUG
    //     std::wcout << L"Numeric folders found:" << std::endl;
    //     for (const auto &folder : numeric_folders)
    //     {
    //         std::wstring wfolder = std::filesystem::path(folder).wstring();
    //         std::wcout << wfolder << std::endl;
    //     }
    // #endif

    for (const auto &folder : numeric_folders)
    {
        std::wcout << L"-----------------------------" << std::endl;
        std::wstring wfolder = std::filesystem::path(folder).wstring();
        std::wcout << L"Processing Folder: " << wfolder << std::endl;
        std::vector<std::string> file_list = FileManager::listDirectory(folder);

        // Split m4s and json files
        auto m4s_files = FileManager::findFilesWithExtension(file_list, ".m4s");
        auto json_files = FileManager::findFilesWithExtension(file_list, ".json");

        // Check if file counts are as expected
        if (m4s_files.size() != 2)
        {
            std::wcerr << L"Error: .m4s file count != 2 in " << wfolder << std::endl;
            continue;
        }

        if (json_files.size() != 1)
        {
            std::wcerr << L"Error: .json file count != 1 in " << wfolder << std::endl;
            continue;
        }

        // Determine the m4s that contains the audio file
        std::string judged_m4s_file;
        if (!MediaProcessor::judgeKeyM4S(m4s_files[0], m4s_files[1], judged_m4s_file))
        {
            continue;
        }

        // Get title from JSON and convert to wstring
        // Preparation for output filename
        std::string title_utf8; // Store title from JSON
        std::wstring wtitle;    // Store converted wstring title

        std::string json_file = json_files[0];
        if (JsonParser::getValueByKey(json_file, "title", title_utf8))
        {
            if (JsonParser::convertToWString(title_utf8, wtitle))
            {
                std::wcout << L"Title: " << wtitle << std::endl;
            }
            else
            {
                std::cerr << "Failed to convert UTF-8 to wstring in folder: " << folder << std::endl;
                continue;
            }
        }
        else
        {
            std::cerr << "Failed to get title from JSON in folder: " << folder << std::endl;
            continue;
        }

        // Process the judged m4s file to mp4
        std::wstring output_name = wtitle; // Use title as output name
        if (!MediaProcessor::processM4sToMp4(judged_m4s_file, output_name, OUTPUT_PATH))
        {
            std::wcerr << L"Failed to process m4s to mp4 in folder: " << wfolder << std::endl;
            continue;
        }
        else
        {
            std::wcout << L"Successfully processed m4s to mp4: " << output_name << L".mp4" << std::endl;
        }
    }

    std::wcout << L"All folders processed. Preparing for MP3 conversion..." << std::endl;
    std::wcout << L"This procedure may take some time, please wait patiently..." << std::endl;
    for (int i = 3; i > 0; --i)
    {
        std::wcout << L"\r                                        \r";
        std::wcout << L"MP3 conversion will start in: " << i << L" seconds..." << std::flush;
        Sleep(1000);
    }
    std::wcout << std::endl;

    // Use ffmpeg to convert all mp4 files in OUTPUT_PATH to mp3
    std::vector<std::string> mp4_files = FileManager::findFilesWithExtension(
        FileManager::listDirectory(OUTPUT_PATH), ".mp4");

    int failed_count = 0;
    for (const auto &mp4_file : mp4_files)
    {
        std::filesystem::path mp4_path = mp4_file;
        std::filesystem::path mp3_path = mp4_path;
        mp3_path.replace_extension(".mp3");

        // Construct ffmpeg command using local ffmpeg.exe
        std::wstring ffmpeg_cmd = L".\\ffmpeg\\ffmpeg.exe -y -i \"";
        ffmpeg_cmd += mp4_path.wstring();
        ffmpeg_cmd += L"\" -vn -acodec libmp3lame -q:a 2 \"";
        ffmpeg_cmd += mp3_path.wstring();
        ffmpeg_cmd += L"\"";

        std::wcout << L"Converting to mp3: " << mp3_path.filename() << std::endl;
        int ret = _wsystem(ffmpeg_cmd.c_str());
        if (ret != 0)
        {
            failed_count++;
            std::wcerr << L"ffmpeg failed for: " << mp4_path.filename() << std::endl;
        }
    }

    // Cleanup temporary files
    for (const auto &mp4_file : mp4_files)
    {
        try
        {
            std::filesystem::remove(mp4_file);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::wcerr << L"Failed to delete temporary file: " << std::filesystem::path(mp4_file).wstring()
                       << L". Error: " << e.what() << std::endl;
        }
    }


    system("cls");
    std::wcout << L"Checklist:" << std::endl;
    std::wcout << L" ----------------------------" << std::endl;
    std::wcout << L"Total valid folders detected: " << numeric_folders.size() << std::endl;
    std::wcout << L"Total mp4 files generated: " << mp4_files.size() << std::endl;
    std::wcout << L"Total mp3 files generated: " << mp4_files.size() - failed_count << std::endl;
    std::wcout << L"----------------------------" << std::endl;
    std::wcout << L"Press any key to exit..." << std::endl;
    std::wcin.get();
    return 0;
}