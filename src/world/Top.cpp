#include "world/Top.hpp"
#include "dir.hpp"
#include <SDL2/SDL_image.h>
#include "main.hpp"

using T = world::Top;

T::Top(){
    CONSTRUCT();
    _image = nullptr;
    scale(1);
}

T::~Top(){
    DESTRUCT();
    if (_image) GPU_FreeImage(_image);
}

bool T::load(std::string path){
    if (path[1] != ':') path = RES + path;
    LOAD_LOG(path);

    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface){
        ERR("IMG_Load : " + std::string(IMG_GetError()));
        return false;
    }

    _image = GPU_CopyImageFromSurface(surface);
    SDL_FreeSurface(surface);
    
    if (!_image){
        ERR("GPU_CopyImageFromSurface");
        return false;
    }
    
    GPU_SetImageFilter(_image, GPU_FILTER_NEAREST);
    return true;
}

GPU_Image *T::image(void) const{
    return _image;
}

const float T::scale(void) const{
    return _scale;
}

void T::scale(const float value){
    _scale = value;
}

void T::scale(std::string str){
    try {
        scale(std::stof(str));
    } catch (std::exception &e){
        ERR("standart exception : " + std::string(e.what()));
    }
}