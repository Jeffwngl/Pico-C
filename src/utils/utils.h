#include <fstream>
#include <sstream>
#include <stdexcept>

std::string readFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path);

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}