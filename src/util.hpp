#pragma once

#include <fstream>
#include <vector>
#include <string>

namespace em_util
{
    std::vector<char> read_file(const std::string& filename);
}
