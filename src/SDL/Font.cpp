#include "SDL/Font.hpp"
#include "dir.hpp"
#include "main.hpp"

Font::Font(){
    font = nullptr;
    size = 16;
}

Font::~Font(){
    destroy_font();
}

bool Font::load(std::string path){
    if (path[1] != ':') path = RES + path;
    LOAD_LOG(path);

    TTF_Font* temp = TTF_OpenFont(path.c_str(), size);

    if (!temp){
        ERR("TTF_OpenFont : " + std::string(TTF_GetError()));
        return false;
    }

    destroy_font();
    font = temp;
    font_path = path;

    size_t lastindex = path.find_last_of("."); 
    name = path.substr(0, lastindex);

    const size_t last_slash_idx = name.find_last_of("\\/");
    if (std::string::npos != last_slash_idx){
        name.erase(0, last_slash_idx + 1);
    }

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

std::string Font::get_name(void){
    return name;
}

SDL_Surface *Font::solid_surface(std::string text, SDL_Color fg){
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), fg);
    if (!surface) ERR("TTF_RenderText_Solid : " + std::string(TTF_GetError()));
    return surface;
}

SDL_Surface *Font::shaded_surface(std::string text, SDL_Color fg, SDL_Color bg){
    SDL_Surface *surface = TTF_RenderText_Shaded(font, text.c_str(), fg, bg);
    if (!surface) ERR("TTF_RenderText_Shaded : " + std::string(TTF_GetError()));
    return surface;
}

SDL_Surface *Font::blended_surface(std::string text, SDL_Color fg){
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), fg);
    if (!surface) ERR("TTF_RenderText_Blended : " + std::string(TTF_GetError()));
    return surface;
}

GPU_Image *Font::solid_image(std::string text, SDL_Color fg){
    SDL_Surface *surface = solid_surface(text, fg);
    if (!surface) return nullptr;
    GPU_Image *image = GPU_CopyImageFromSurface(surface);
    SDL_FreeSurface(surface);
    return image;
}

GPU_Image *Font::shaded_image(std::string text, SDL_Color fg, SDL_Color bg){
    SDL_Surface *surface = shaded_surface(text, fg, bg);
    if (!surface) return nullptr;
    GPU_Image *image = GPU_CopyImageFromSurface(surface);
    SDL_FreeSurface(surface);
    return image;
}

GPU_Image *Font::blended_image(std::string text, SDL_Color fg){
    SDL_Surface *surface = blended_surface(text, fg);
    if (!surface) return nullptr;
    GPU_Image *image = GPU_CopyImageFromSurface(surface);
    SDL_FreeSurface(surface);
    return image;
}

bool Font::set_size(int size){
    if (size <= 0){
        WARN("the input size is negative or nul " + std::to_string(size) + " <= 0, set as 1");
        size = 1;
    }
    this->size = size;
    return reload();
}