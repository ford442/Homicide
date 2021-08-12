#include <Windows.h>
#include <filesystem>
#include "Game.hpp"
#include "dir.hpp"

void HideConsole(){
    ::ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void ShowConsole(){
    ::ShowWindow(GetConsoleWindow(), SW_SHOW);
}

std::string getCurrentDateTime(std::string s){
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];

    tstruct = *localtime(&now);
    if(s=="now")
        strftime(buf, sizeof(buf), "%Y-%m-%d-%X", &tstruct);
    else if(s=="date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

    return std::string(buf);
}

std::string log_dir(void){
    return LOGS + getCurrentDateTime("date");
}

bool initLogDir(void){
    std::cout << log_dir() << std::endl;
    return std::filesystem::create_directories(log_dir());
}

bool Init_logs(void){
    GPU_SetDebugLevel(GPU_DEBUG_LEVEL_3);

    if (!std::filesystem::exists(log_dir())){
        if (!initLogDir()){
            std::cerr << "ERROR :: cannot create the log folder" << std::endl;
            return false;
        }
    }

    std::string log_path = log_dir() + "\\log_" + getCurrentDateTime("date") + ".log";
    freopen(log_path.c_str(), "w", stdout);

    std::cout << "LOG file :: init at " << getCurrentDateTime("now") << std::endl;
 
    std::string err_path = log_dir() + "\\err_" + getCurrentDateTime("date") + ".log";
    std::cout << "open err file : \"" << err_path << "\"" << std::endl;
    freopen(err_path.c_str(), "w", stderr);

    std::cerr << "ERROR file :: init at " << getCurrentDateTime("now") << std::endl;
    
    return true;
}

int main(int argc, char **argv){

    #ifndef SHOW_TERMINAL
        HideConsole();
    #endif

    Init_logs();
    Game homicide;

    if (homicide.Init_libs())
        homicide.run();

    homicide.quit();

    std::cout << "programe end" << std::endl;
    return 0;
}
