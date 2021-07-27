#include "weapons/Projectile_type.hpp"
#include <string>

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

bool Projectile_type::load(XMLNode *node, std::shared_ptr<light::LightImageList> lights){
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (!strcmp(attr.key, "name")){
            name = attr.value;
        } else {
            WARN("cannot reconize : \"" + std::string(attr.key) + "\"");
        }
    }

    if (name == "unknown"){
        ERR("the name of the projectil is not valid");
        return false;
    }

    for (int c=0; c<node->children.size; c++){
        XMLNode *child = XMLNode_child(node, c);

        if (!strcmp(child->tag, "light")){
            for (int a=0; a<child->attributes.size; a++){
                XMLAttribute attr = child->attributes.data[a];

                if (!strcmp(attr.key, "type")){
                    light = lights->get(attr.value);
                }
            }
        } else if (!strcmp(child->tag, "damages")){
            for (int a=0; a<child->attributes.size; a++){
                XMLAttribute attr = child->attributes.data[a];

                if (!strcmp(attr.key, "damages")){
                    try{
                        damages = std::stoi(attr.value);
                    } catch (std::exception &e){
                        ERR("standart exception : " + std::string(e.what()));
                        damages = 0;
                    }
                } else {
                    WARN("cannot reconize \"" + std::string(attr.key) + "\"");
                }
            }
        } else {
            WARN("cannot reconize \"" + std::string(child->tag) + "\"");
        }
    }

    return true;
}