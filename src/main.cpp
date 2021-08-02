#include <Windows.h>
#include "Game.hpp"
#include "dir.hpp"

void HideConsole(){
    ::ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void ShowConsole(){
    ::ShowWindow(GetConsoleWindow(), SW_SHOW);
}

int main(int argc, char **argv){

    #ifndef SHOW_TERMINAL
        HideConsole();
    #endif

    Game homicide;

    if (homicide.Init_libs())
        homicide.run();

    homicide.quit();

    std::cout << "programe end" << std::endl;
    return 0;
}
