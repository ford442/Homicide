#include <windows.h>
#include "Game.hpp"
#include "dir.hpp"

#define HIDE_TERMINAL true

void HideConsole(){
    ::ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void ShowConsole(){
    ::ShowWindow(GetConsoleWindow(), SW_SHOW);
}

int main(int argc, char **argv){
    
    if (HIDE_TERMINAL) HideConsole();

    Game homicide;

    if (homicide.Init_libs())
        homicide.run();

    homicide.quit();

    std::cout << "programe end" << std::endl;
    return 0;
}
