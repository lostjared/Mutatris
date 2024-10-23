#include"util.hpp"

namespace util {

#ifdef FOR_WASM
std::string path = "/assets";
#else
std::string path = "assets";
#endif

std::string getFilePath(const std::string &filename) {
    std::ostringstream stream;
    stream << path << "/" << filename;
    return stream.str();
}

}