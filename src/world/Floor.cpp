#include "world/Floor.hpp"
#include "dir.hpp"
#include <SDL2/SDL_image.h>
#include "main.hpp"

using F = world::Floor;

F::Floor(){
    CONSTRUCT();
    _image = nullptr;
    scale(1);
}

F::~Floor(){
    DESTRUCT();
    if (_image) GPU_FreeImage(_image);
}

bool F::load(std::string path){
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

GPU_Image *F::image(void) const{
    return _image;
}

const float F::scale(void) const{
    return _scale;
}

void F::scale(const float value){
    _scale = value;
}

void F::scale(std::string str){
    try {
        scale(std::stof(str));
    } catch (std::exception &e){
        ERR("standart exception : " + std::string(e.what()));
    }
}