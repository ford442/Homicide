#include "Game.hpp"
#include "dir.hpp"

int main(int argc, char **argv){
    Game homicide;

    if (homicide.Init_libs())
        homicide.run();

    homicide.quit();

    std::cout << "programe end" << std::endl;
    return 0;
}