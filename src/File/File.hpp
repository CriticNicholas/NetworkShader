#pragma once

#include<filesystem>
#include<fstream>
#include<iostream>

class File{
    public:
        File(std::filesystem::path FilePath) : FilePath(FilePath) {};
        ~File() { InputFileInterface.close(); } 

        std::string ReadFile()
        {
            try
            {

                auto FileSize = std::filesystem::file_size(FilePath);

                std::string OutputString(FileSize, '\0');
                InputFileInterface.read(OutputString.data(), FileSize);

                return OutputString;
            }

            catch(const std::filesystem::filesystem_error& e)
            {
                std::cerr << e.what();
                return "\0";
            }
        }
    private:
        std::filesystem::path FilePath;
        std::ifstream InputFileInterface{FilePath, std::ios::binary};
};
