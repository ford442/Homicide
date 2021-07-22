#include "lights/LightImage.hpp"
#include "dir.hpp"
#include <SDL2/SDL_image.h>
#include <cstring>

using L = light::LightImage;

L::LightImage(float *pixel_size, float *x, float *y) : _pixel_size(pixel_size), _x(x), _y(y){
    std::cout << "INFO :: allocating a LightImage instance" << std::endl;
    _image = nullptr;
    _scale = 1;
    center = {-1, -1};
}

L::LightImage(XMLNode* node, float *pixel_size, float *x, float *y) : _pixel_size(pixel_size), _x(x), _y(y){
    std::cout << "INFO :: allocating a LightImage instance" << std::endl;
    _scale = 1;
    load(node);
    center = {-1, -1};
}

L::LightImage(std::string path, std::string name, float *pixel_size, float *x, float *y) : _pixel_size(pixel_size), _x(x), _y(y){
    std::cout << "INFO :: allocating a LightImage instance" << std::endl;
    _scale = 1;
    load(path, name);
    center = {-1, -1};
}

L::~LightImage(){
    std::cout << "INFO :: releasing memory from a LightImage instance" << std::endl;
    if (_image) GPU_FreeImage(_image);
}

bool L::load(std::string path, std::string name){
    _name = name;
    return load(path);
}

bool L::load(XMLNode* node){
    std::cout << "INFO :: load light images" << std::endl;
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (!strcmp(attr.key, "name")){
            _name = attr.value;

        } else if (!strcmp(attr.key, "path")){
            if(!load(attr.value)) return false;

        } else if (!strcmp(attr.key, "center-x")){
            sscanf(attr.value, "%f", &center.x);

        } else if (!strcmp(attr.key, "center-y")){
            sscanf(attr.value, "%f", &center.y);
        
        } else if (!strcmp(attr.key, "scale")){
            sscanf(attr.value, "%f", &_scale);

        } else {
            std::cerr << "WARNING :: cannot reconize \"" << attr.key << "\" light-image attribute" << std::endl;
        }
    }

    if (!_image){
        std::cerr << "ERROR :: cannot create a light image with a none light image" << std::endl;
        return false;
    }

    if (_name.empty()){
        std::cerr << "ERROR :: cannot create a light image without a name" << std::endl;
        return false;
    }

    std::cout << "new light surface, name : \"" << _name << "\"" << std::endl;
    return true;
}

GPU_Image* L::get_image(void) const{
    return _image;
}

std::string L::get_name(void) const{
    return _name;
}

bool L::load(std::string path){
    if (path[1] != ':') path = RES + path;
    std::cout << "INFO :: load \"" << path << "\"" << std::endl;

    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface){
        std::cerr << "ERROR :: IMG_Load : " << IMG_GetError() << " file : \"" << path << "\"" << std::endl;
        return false;
    }

    _image = GPU_CopyImageFromSurface(surface);
    SDL_FreeSurface(surface);
    return true;
}


void L::draw(GPU_Target* t, const int x, const int y, const float angle){
    if (center.x != -1 && center.y != -1){
        GPU_BlitTransformX(_image, nullptr, t, x, y, center.x, center.y, angle, _scale, _scale);
    } else {
        GPU_BlitTransform(_image, nullptr, t, x, y, angle, _scale, _scale);
    }
}