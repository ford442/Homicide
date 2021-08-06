#include "world/Top.hpp"
#include "dir.hpp"
#include <SDL2/SDL_image.h>
#include "main.hpp"

using T = world::Top;

T::Top(){
    CONSTRUCT();
    _image = nullptr;
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
    return true;
}

GPU_Image *T::image(void) const{
    return _image;
}