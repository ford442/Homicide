#include "lights/Light.hpp"
#include "csv.hpp"
#include "main.hpp"
#include <cstring>

Light::Light(){
    shadow_target = nullptr;
    shadow_image = nullptr;
}

Light::~Light(){
    destroy_shadow();
}

bool Light::load(XMLNode *node){
    if (!node) return false;

    int w=-1, h=-1;

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "w") || is_equal(attr.key, "width")){
            try{
                w = std::stoi(attr.value);
            } catch (std::exception &e){
                ERR("standart exception : " + std::string(e.what()));
                w = -1;
            }
        } else if (is_equal(attr.key, "h") || is_equal(attr.key, "height")){
            try{
                h = std::stoi(attr.value);
            } catch (std::exception &e){
                ERR("standart excpeiotn : " + std::string(e.what()));
                h = -1;
            }
        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" " + std::string(node->tag) + " attribute");
        }
    }

    if (w <= 0){
        ERR("the width if the layer layer is invalid, the value is negative or nul" + std::to_string(w) + " <= 0");
        return false;
    }

    if (h <= 0){
        ERR("the height of the light layer is invalid, the value if negatove or nul" + std::to_string(h) + " <= 0");
        return false;
    }
}

bool Light::load(std::string path){

}

GPU_Image *Light::get_image(void) const{
    return shadow_image;
}

GPU_Target *Light::get_target(void) const{
    return shadow_target;
}

void Light::destroy_shadow(void){
    if (shadow_target){
        GPU_FreeTarget(shadow_target);
        shadow_target = nullptr;
    }

    if (shadow_image){
        GPU_FreeImage(shadow_image);
        shadow_image = nullptr;
    }
}

bool Light::uupdate_size(int &w, int &h){

    if (w <= 0 || h <= 0){
        return false;
    }

    destroy_shadow();

    shadow_image = GPU_CreateImage(w, h, GPU_FORMAT_RGBA);

    if (!shadow_image){
        return false;
    }

    shadow_target = GPU_LoadTarget(shadow_image);
    return shadow_target;
}