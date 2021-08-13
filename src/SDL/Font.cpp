#include "SDL/Font.hpp"
#include "dir.hpp"
#include "main.hpp"

Font::Font(){
    font = nullptr;
}

Font::~Font(){
    destroy_font();
}

bool Font::load(std::string path){
    if (path[1] != ':') path = RES + path;

    TTF_Font* temp = TTF_OpenFont(path.c_str(), size);

    if (!temp){
        ERR("TTF_OpenFont : " + std::string(TTF_GetError()));
        return false;
    }

    destroy_font();
    font = temp;
    font_path = path;

    return true;
}

TTF_Font* Font::get_font(void){
    return font;
}

void Font::destroy_font(void){
    if (font){
        TTF_CloseFont(font);
        font = nullptr;
    }
}

bool Font::reload(void){
    return load(font_path);
}