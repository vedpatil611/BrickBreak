#include "Utils.h"

std::string Utils::readFile(const char* filepath)
{
    FILE* file = fopen(filepath, "rt");
    
    fseek(file, 0, SEEK_END);
    auto length = ftell(file);
    std::string data(length + 1, 0);
    fseek(file, 0, SEEK_SET);
    fread(data.data(), 1, length, file);
    fclose(file);

    return data;
}
