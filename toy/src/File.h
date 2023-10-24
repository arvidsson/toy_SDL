#pragma once

#include "Core.h"
#include <fstream>

namespace toy
{

class File
{
public:
    static Result<String> load(const String& path)
    {
        std::ifstream file(path);
        if (!file.is_open()) return Result(format("Failed to open file: {}", path));
        if (!file.good()) return Result(format("Something went wrong trying to read file: {}", path));
        std::stringstream sstream;
        sstream << file.rdbuf();
        file.close();
        return sstream.str();
    }
};

}