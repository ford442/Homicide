#include "widgets/Button.hpp"
#include "main.hpp"

Button::Button(){
    path = "";
}

Button::~Button(){

}

const std::string Button::get_path(void) const{
    return path;
}

void Button::set_path(const std::string path){
    this->path = path;
}

bool Button::load(XMLNode *node){
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "file_path")) path = attr.value;
    }

    if (path.empty()){
        ERR("cannot found the \"file_path\" attribute in " + std::string(node->tag) + " node");
        return false;
    }

    return true;
}