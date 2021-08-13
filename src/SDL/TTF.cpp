#include "SDL/TTF.hpp"
#include "main.hpp"
#include "dir.hpp"
#include <vector>

TTF::TTF(){
    is_ttf_init = false;
}

TTF::~TTF(){
    fonts.clear();
}

bool TTF::init(void){
    if (TTF_Init() == -1){
        ERR("TTF_Init : " + std::string(TTF_GetError()));
        is_ttf_init = false;
    } else {
        is_ttf_init = true;
    }
    return is_init();
}

bool TTF::load_font(std::string path){
    Font font;

    if (font.load(path)){
        fonts.push_back(font);
    } else {
        return false;
    }
    return true;
}

bool TTF::is_init(void) const{
    return is_ttf_init;
}

bool TTF::load_font_dir(std::string dir){
    if (dir[1] != ':') dir = RES + dir;
    LOAD_LOG(dir);
    std::vector<std::string> files = dir::content(dir);

    if (files.empty()){
        ERR("the given directory \"" + dir + "\" not exist");
        return false;
    }

    for (auto &f : files){
        if (f == ".." || f == ".") continue;
        load_font(dir + f);
    }

    if (fonts.empty()){
        ERR("the givent directory \"" + dir + "\" is empty");
        return false;
    }

    return true;
}