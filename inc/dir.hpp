#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace dir{
    std::string ResourcesDir(void);
    std::string LogDir(void);
    std::string DataDir(void);
    std::vector<std::string> content(std::string directory_path);
}

#ifndef RES
    #define RES dir::ResourcesDir()
#endif

#ifndef LOGS
    #define LOGS dir::LogDir()
#endif

#ifndef DATA
    #define DATA dir::DataDir()
#endif