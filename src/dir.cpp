#include "dir.hpp"
#include <dirent.h>
#include <wtypes.h>
#include <windows.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <limits.h>

// #define _WIN32_WINNT 0x0502

std::vector<std::string> dir::content(std::string directory_path){
    DIR *dh;
    struct dirent * contents;
    std::vector<std::string> content;

    dh = opendir(directory_path.c_str());

    if (!dh){
        std::cerr << "The given directory is not found" << std::endl;
        return content;
    }


    while ((contents = readdir(dh)) != NULL){
        std::string name = contents->d_name;
        content.push_back(name);
    }
    closedir ( dh );
    return content;
}

std::string getCurrentDir(void){
    char buff[MAX_PATH];
    GetModuleFileName( NULL, buff, MAX_PATH );
    std::string::size_type position = std::string( buff ).find_last_of( "\\/" );
    return std::string( buff ).substr( 0, position);
}

std::string getMainDir(void){
    std::string buff = getCurrentDir();
    std::size_t pos = buff.find("bin");

    if (pos != std::string::npos)
        buff = buff.erase(pos);

    return buff;
}

std::string dir::ResourcesDir(void){
    return getMainDir() + "res\\";
}

std::string dir::LogDir(void){
    return std::string(getenv("AppData")) + "\\Homicide\\logs\\";
}

std::string dir::DataDir(void){
    return getMainDir() + "res\\data\\";
}

std::string dir::main_dir(void){
    return getMainDir();
}