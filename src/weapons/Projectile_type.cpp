#include "weapons/Projectile_type.hpp"
#include <string>
#include "csv.hpp"

#define WARNS
#define ERRS

#ifdef WARNS
    #define WARN(msg) std::cerr << "WARNING :: " << __func__ << " : " << msg << std::endl;
#else
    #define WARN(msg)
#endif

#ifdef ERRS
    #define ERR(msg) std::cerr << "ERROR :: " << __func__ << " : " << msg << std::endl;
#else
    #define ERR(msg)
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
        speed = std::stoi(doc.search("damages"));
    } catch (std::exception& e){
        ERR("standart exception : " + std::string(e.what()));
        speed = 10;
    }

    return true;
}