#include "weapons/Weapon_type.hpp"
#include "dir.hpp"
#include <SDL2/SDL_image.h>
#include <cstring>

using W = weapons::Weapon_type;

#define CONSTRUCT_LOGS
#define DECONSTRUCT_LOGS
#define INIT_LOGS
#define INIT_WARNS
#define INIT_ERRS

#ifdef CONSTRUCT_LOGS
    #define CONS_LOGS(value) \
        value;
#else
    #define CONS_LOGS(value) {}
#endif

#ifdef DECONSTRUCT_LOGS
    #define DECONS_LOGS(value) \
        value;
#else
    #define DECONS_LOGS(value) {}
#endif 

#ifdef INIT_LOGS
    #define INIT_LOG(value) \
        value;
#else
    #define INIT_LOG(value) {}
#endif 

#ifdef INIT_WARNS
    #define INIT_WARN(value) \
        value;
#else
    #define INIT_WARN(value) {}
#endif 

#ifdef INIT_ERRS
    #define INIT_ERR(value) \
        value;
#else
    #define INIT_ERR(value) {}
#endif 

W::Weapon_type(){
    CONS_LOGS(std::cout << "INFO :: Weapon_type : allocating a new Weapon_type instance" << std::endl);
    _name = "unknown";
}

W::~Weapon_type(){
    DECONS_LOGS(std::cout << "INFO :: Weapon_type : releasing memory from a new Weapon_type instance" << std::endl);
}

bool W::Init(std::string path){
    if (path[1] != ':') path = RES + path;

    INIT_LOG(std::cout << "INFO :: Weapon_type : init weapon from \"" << path << "\"" << std::endl);

    XMLDocument doc;

    if (XMLDocument_load(&doc, path.c_str())){
        for (int c=0; c<doc.root->children.size; c++){
            XMLNode *child = XMLNode_child(doc.root, c);

            if (!strcmp(child->tag, "name")) set_name_xml(child);
        }
    } else {
        INIT_ERR(std::cout << "ERROR :: Weapon_type : cannot load the xml file" << std::endl);
        return false;
    }

    return true;
}

std::string W::get_name(void) const{
    return _name;
}

void W::set_name(std::string name){
    _name = name;
}

void W::set_name_xml(XMLNode* node){
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (!strcmp(attr.key, "name")){
            set_name(attr.value);
        } else {
            INIT_WARN(std::cerr << "WARNING :: Weapon_type : cannot reconize \"" << attr.key << "\" name attribute" << std::endl);
        }
    }
}

void W::set_item_image_xml(XMLNode *node){
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (!strcmp(attr.key, "image")){
            set_item_image(attr.value);
        }
    }
}

bool W::set_item_image(std::string path){
    if (path[1] == ':') path = RES + path;
    SDL_Surface *surface = IMG_Load(path.c_str());

    if (!surface){
        INIT_ERR(std::cerr << "ERROR :: Weapon_type : cannot load image : " << IMG_GetError() << ", file : \"" << path << "\"" << std::endl);
        return false;
    }

    item_img = GPU_CopyImageFromSurface(surface);
    SDL_FreeSurface(surface);
    return true;
}