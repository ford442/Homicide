#include "weapons/Projectile_type.hpp"
#include <string>
#include <SDL2/SDL_image.h>
#include <cmath>
#include "dir.hpp"
#include "csv.hpp"

#define _WARNS true
#define _ERRS true
#define _LOGS true

#ifdef _WARNS
    #define WARN(msg) std::cerr << "WARNING :: " << __func__ << " : " << msg << std::endl;
#else
    #define WARN(msg)
#endif

#ifdef _ERRS
    #define ERR(msg) std::cerr << "ERROR :: " << __func__ << " : " << msg << std::endl;
#else
    #define ERR(msg)
#endif

#ifdef _LOGS
    #define LOG(msg) std::cout << "INFO :: " << __func__ << " : " << msg << std::endl;
#else
    #define LOG(msg)
#endif

Projectile_type::Projectile_type(){
    image = nullptr;
    light = nullptr;
    name = "unknown";
}

Projectile_type::~Projectile_type(){

}

GPU_Image *Projectile_type::get_image(void) const{
    return image;
}

bool Projectile_type::load(std::string path, std::shared_ptr<light::LightImageList> lights){
    CSV::Document doc;

    if (!doc.load(path)) return false;

    name = doc.search("name");

    try {
        damages = std::stoi(doc.search("damages"));
    } catch (std::exception& e){
        ERR("standart exception : " + std::string(e.what()));
        damages = 10;
    }

    try {
        max_speed = std::stof(doc.search("max_speed"));
    } catch (std::exception& e){
        ERR("standart exception : " + std::string(e.what()));
        max_speed = 10;
    }

    try {
        min_speed = std::stof(doc.search("min_speed"));
    } catch (std::exception& e){
        ERR("standart exception : " + std::string(e.what()));
        min_speed = 10;
    }

    light = lights->get(doc.search("light"));
    if (!load_img(doc.search("image"))) return false;

    return true;
}

int Projectile_type::get_damages(void) const{
    return damages;
}

float Projectile_type::get_max_speed(void) const{
    return max_speed;
}

float Projectile_type::get_min_speed(void) const{
    return min_speed;
}

float Projectile_type::get_rand_speed(void) const{
    return std::fmod(rand(), max_speed - min_speed) + min_speed;
}

std::shared_ptr<light::LightImage> Projectile_type::get_light_image(void){
    return light;
}

bool Projectile_type::load_img(std::string path){
    if (path[1] != ':') path = RES + path;

    SDL_Surface *surface = IMG_Load(path.c_str());

    if (!surface){
        ERR("IMG_Load : " + std::string(IMG_GetError()) + ", file : \"" + path + "\"");
        return false;
    }

    image = GPU_CopyImageFromSurface(surface);
    SDL_FreeSurface(surface);

    if (!image){
        ERR("GPU_CopyImageFromSurface");
        return false;
    }
    return true;
}

float Projectile_type::get_caliber(void) const{
    return caliber;
}

bool Projectile_type::load_icon(std::string path){
    if (path[1] != ':') path = RES + path;

    SDL_Surface *surface = IMG_Load(path.c_str());

    if (!surface){
        ERR("IMG_Load : " + std::string(IMG_GetError()) + ", file : \"" + path + "\"");
        return false;
    }

    icon = GPU_CopyImageFromSurface(surface);
    SDL_FreeSurface(surface);

    if (!icon){
        ERR("GPU_CopyImageFromSurface");
        return false;
    }
    return true;
}


GPU_Image *Projectile_type::get_icon(void) const{
    return icon;
}