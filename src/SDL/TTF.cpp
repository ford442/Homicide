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
    std::shared_ptr<Font> font = std::make_shared<Font>();

    if (font->load(path)){

        if (!get(font->get_name())){
            fonts.push_back(font);
        } else {
            ERR("the name \"" + font->get_name() + "\" is early used by another font");
            return false;
        }
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

std::shared_ptr<Font> TTF::get(std::string name){
    for (auto &f : fonts){
        if (f->get_name() == name){
            return f;
        }
    }
    return nullptr;
}

std::list<std::shared_ptr<Font>> TTF::get_fonts(void) const{
    return fonts;
}