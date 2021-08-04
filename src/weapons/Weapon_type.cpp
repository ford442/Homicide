#include "weapons/Weapon_type.hpp"
#include "xml.hpp"
#include <cstring>
#include <SDL2/SDL_image.h>
#include "dir.hpp"

#define is_equal(str1, str2) !strcmp(str1, str2)

#define WARN(msg) std::cerr << "WARNING :: in " << __FILE__ << " at " << __func__ << " : " << msg << std::endl;
#define ERR(msg) std::cerr << "ERROR :: in " << __FILE__ << " at " << __func__ << " : " << msg << std::endl;
#define LOG(msg) std::cout << "INFO :: in " << __FILE__ << " at " << __func__ << " : " << msg << std::endl;

using namespace weapons;

static inline void to_float(std::string str, float *i){
    try {
        *i = std::stof(str);
    } catch (std::exception &e){
        ERR("standart execption : " + std::string(e.what()));
        *i = 0;
    }
}

static inline void to_int(std::string str, int *i){
    try {
        *i = std::stoi(str);
    } catch (std::exception &e){
        ERR("standart execption : " + std::string(e.what()));
        *i = 0;
    }
} 

Weapon_type::Weapon_type(){
    image = nullptr;
}

Weapon_type::~Weapon_type(){
    destroy_image();
}

bool Weapon_type::load(std::string path){
    LOG("load weapon type from \"" + path + "\"");

    XMLDocument doc;
    if (XMLDocument_load(&doc, path.c_str())){
        for (int c=0; c<doc.root->children.size; c++){
            XMLNode *child = XMLNode_child(doc.root, c);

            if (is_equal(child->tag, "weapon")){
                load_node(child);

            } else {
                WARN("cannot reconize \"" + std::string(child->tag) + "\" in \"" + path + "\"");
            }
        }
    } else {
        return false;
    }

    XMLDocument_free(&doc);
    return true;
}

std::string Weapon_type::get_name(void) const{
    return _name;
}

void Weapon_type::set_name(std::string name){
    _name = name;
}

void Weapon_type::set_name_xml(XMLNode* node){
    LOG("set weapon name from xml");
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "name")){
            _name = attr.value;
        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" weapon attribute");
        }
    }
    LOG("name set to \"" + _name + "\"");
}

void Weapon_type::set_image_xml(XMLNode *node){
    LOG("set the image from an xml node");
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "path")){
            set_image(attr.value);
        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" weapon image attribute");
        }
    }
}

bool Weapon_type::set_image(std::string path){
    destroy_image();
    if (path[1] != ':') path = RES + path; 

    SDL_Surface *surface = IMG_Load(path.c_str());

    if (!surface){
        ERR("IMG_Load : " + std::string(IMG_GetError()));
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

void Weapon_type::destroy_image(void){
    if (image){
        GPU_FreeImage(image);
        image = nullptr;
    }
}

void Weapon_type::load_node(XMLNode *node){
    set_name_xml(node);
    
    for (int c=0; c<node->children.size; c++){
        XMLNode *child = XMLNode_child(node, c);

        if (is_equal(child->tag, "image")){
            set_image_xml(child);
        
        } else if (is_equal(child->tag, "position")){
            load_position(child);
        
        } else if (is_equal(child->tag, "power")){
            load_power(child);
        
        } else if (is_equal(child->tag, "countdown")){
            load_countdown(child);
        
        } else if (is_equal(child->tag, "cannon-pos")){
            load_cannon_pos(child);
        
        } else if (is_equal(child->tag, "precision")){
            load_precision(child);
        
        } else if (is_equal(child->tag, "mass")){
            load_mass(child);
        
        } else if (is_equal(child->tag, "recoil")){
            load_recoil(child);
        
        } else if (is_equal(child->tag, "caliber")){
            load_caliber(child);

        } else {
            WARN("cannot reconize \"" + std::string(child->tag) + "\" weapon child");
        }
    }

    LOG("weapon loaded");
}

void Weapon_type::load_position(XMLNode *node){
    LOG("set the position from an xml node");

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "x")){
            to_float(attr.value, &x);
        
        } else if (is_equal(attr.key, "y")){
            to_float(attr.value, &y);

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" weapon position attribute");
        }
    }
}

void Weapon_type::load_power(XMLNode *node){
    LOG("set the power from an xml node");

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "power")){
            to_float(attr.value, &power);

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" weapon power attribute");
        }
    }
}

void Weapon_type::load_countdown(XMLNode *node){
    LOG("set the countdown from an xml node");

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "countdown")){
            to_int(attr.value, &countdown);

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" weapon countdown attribute");
        }
    }
}

void Weapon_type::load_cannon_pos(XMLNode *node){
    LOG("set the position from an xml node");

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "x")){
            to_float(attr.value, &cannon_x);
        
        } else if (is_equal(attr.key, "y")){
            to_float(attr.value, &cannon_y);

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" weapon position attribute");
        }
    }
}

void Weapon_type::load_precision(XMLNode *node){
    LOG("set the precision from an xml node");

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "deg")){
            to_float(attr.value, &precision);

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" weapon precision attribute");
        }
    }
}

void Weapon_type::load_mass(XMLNode *node){
    LOG("set the mass from an xml node");

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "mass")){
            to_float(attr.value, &mass);

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" weapon mass attribute");
        }
    }
}

void Weapon_type::load_recoil(XMLNode *node){
    LOG("set the recoil from an xml node");

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "recoil")){
            to_float(attr.value, &recoil);

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" weapon recoil attribute");
        }
    }
}

void Weapon_type::load_caliber(XMLNode *node){
    LOG("set the caliber from an xml node");

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "caliber")){
            to_float(attr.value, &caliber);

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" weapon caliber attribute");
        }
    }
}

bool Weapon_type::is_valid_projecile(std::shared_ptr<Projectile_type> p){
    return (p->get_caliber() == caliber);
}