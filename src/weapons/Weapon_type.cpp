#include "weapons/Weapon_type.hpp"
#include "xml.hpp"
#include <cstring>

using namespace weapons;

Weapon_type::Weapon_type(){
    image = nullptr;
}

Weapon_type::~Weapon_type(){
    destroy_image();
}

bool Weapon_type::load(std::string path){
    XMLDocument doc;
    
    if (!XMLDocument_load(&doc, path.c_str())){
        for (int c=0; c<doc.root->children.size; c++){
            XMLNode *child = XMLNode_child(doc.root, c);

        }
    } else {
        return false;
    }

    XMLDocument_free(&doc);
    return false;
}

std::string Weapon_type::get_name(void) const{

}

void Weapon_type::set_name(std::string name){

}

void Weapon_type::set_name_xml(XMLNode* node){

}

void Weapon_type::set_image_xml(XMLNode *node){

}

bool Weapon_type::set_image(std::string path){
    
}

void Weapon_type::destroy_image(void){
    if (image){
        GPU_FreeImage(image);
        image = nullptr;
    }
}