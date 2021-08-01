#include "lights/LightImage.hpp"
#include "dir.hpp"
#include <SDL2/SDL_image.h>
#include <cstring>
#include <csv.hpp>

#define _ERR true
#define _LOG true
#define _WARN true

#ifdef _ERR
    #define ERR(msg) std::cerr << "ERROR :: \"" << __func__ << "\" in \"" << __FILE__ << "\" : " << msg << std::endl;
#else
    #define ERR(msg)
    #define _ERR false
#endif

#ifdef _LOG
    #define LOG(msg) std::cout << "INFO :: \"" << __func__ << "\" in \"" << __FILE__ << "\" : " << msg << std::endl;
#else
    #define LOG(msg)
    #define _LOG false
#endif

#ifdef _WARN
    #define WARN(msg) std::cerr << "WARNING :: \"" << __func__ << "\" in \"" << __FILE__ << "\" : " << msg << std::endl;
#else
    #define WARN(msg)
    #define _WARN false
#endif

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

bool L::load_csv(std::string path){
    CSV::Document doc;

    doc.set_custom_Debug(_ERR, _LOG, _WARN);
    if (!doc.load(path)) return false;

    _name = doc.search("name");
    if (_name == ""){
        ERR("non valid name in \"" + path + "\"");
        return false;
    }

    if (!load(doc.search("image"))){
        ERR("cannot load the image in \"" + path + "\"");
        return false;
    }

    try {
        center.x = std::stoi(doc.search("center-x"));
    } catch (std::exception &e){
        WARN("standart exception : " + std::string(e.what()));
        center.x = _image->w / 2;
    }

    try {
        center.y = std::stoi(doc.search("center-y"));
    } catch (std::exception &e){
        WARN("standart exception : " + std::string(e.what()));
        center.y = _image->h / 2;
    }

    return true;
}