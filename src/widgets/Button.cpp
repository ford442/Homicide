#include "widgets/Button.hpp"

Button::Button(){
    path = "unknown";
}

Button::~Button(){

}

const std::string Button::get_path(void) const{
    return path;
}

void Button::set_path(const std::string path){
    this->path = path;
}