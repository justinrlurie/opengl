#include "file_io.h"

std::string io_Write_to_string(const std::string input_file_path)
{
    std::fstream input_file;
    input_file.open(input_file_path, std::ios::in);

    std::string file_text;
    if (input_file.is_open())
    {
        std::string line;
        while (getline(input_file, line))
        {
            file_text += (line + "\n");
        }
    }

    return file_text;
}