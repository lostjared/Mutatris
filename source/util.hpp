#ifndef __UTIL_H__
#define __UTIL_H__

#include<iostream>
#include<sstream>
#include<string>

namespace util  {

extern std::string path;
std::string getFilePath(const std::string &filename);

}

#endif