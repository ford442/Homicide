#include "world/Floor.hpp"
#include "dir.hpp"
#include <SDL2/SDL_image.h>

using F = world::Floor;

F::Floor(float *pixel_size) : _pixel_size(pixel_size){
    std::cout << "INFO :: allocating a Floor instance" << std::endl;
    _image = nullptr;
}

F::~Floor(){
    std::cout << "INFO :: releasing memory from a Floor instance" << std::endl;
    if (_image) GPU_FreeImage(_image);
}

bool F::load(std::string path){
    if (path[1] != ':') path = RES + path;
    std::cout << "INFO :: load \"" << path << "\"" << std::endl;

    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface){
        std::cerr << "ERROR :: IMG_Load : " << IMG_GetError()<< " file : \"" << path << "\"" << std::endl;
        return false;
    }

    _image = GPU_CopyImageFromSurface(surface);

    SDL_FreeSurface(surface);
    return true;
}

void F::draw(GPU_Target* t){
    GPU_Blit(_image, nullptr, t, _image->w / 2, _image->h / 2);
}