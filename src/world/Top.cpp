#include "world/Top.hpp"
#include "dir.hpp"
#include <SDL2/SDL_image.h>

using T = world::Top;

T::Top(float *pixel_size, float *x, float *y) : _pixel_size(pixel_size), _x(x), _y(y){
    std::cout << "INFO :: allocating a Top insantce" << std::endl;
    _image = nullptr;
}

T::~Top(){
    std::cout << "INFO :: releasing memory from a Top instance" << std::endl;
    if (_image) GPU_FreeImage(_image);
}

bool T::load(std::string path){
    if (path[1] != ':') path = RES + path;
    std::cout << "INFO :: load \"" << path << "\"" << std::endl;

    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface){
        std::cerr << "ERROR :: IMG_Load : " << IMG_GetError()<< " file : \"" << path << "\"" << std::endl;
        return false;
    }

    _image = GPU_CopyImageFromSurface(surface);
    GPU_SetImageFilter(_image, GPU_FILTER_NEAREST);

    SDL_FreeSurface(surface);
    return true;
}

void T::draw(GPU_Target* t){
    _image->base_w = _image->w * *_pixel_size;
    _image->base_h = _image->h * *_pixel_size;
    GPU_Rect src = {0, 0, float(_image->base_w), float(_image->base_h)};
    GPU_Blit(_image, &src, t, -*_x + (_image->w * *_pixel_size) / 2, -*_y + (_image->h * *_pixel_size) / 2);
}