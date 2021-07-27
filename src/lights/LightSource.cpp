#include "lights/LightSource.hpp"

using LS = light::LightSource;

LS::LightSource(){
    _color = {0, 0, 0, 0};
    _isOn = true;
    _image = nullptr;
    _calculation_image = nullptr;
}

LS::~LightSource(){
    
}

const float LS::x(void) const{
    return _x;
}

const float LS::y(void) const{
    return _y;
}

void LS::pos(int *x, int *y) const{
    *x = this->x();
    *y = this->y();
}

void LS::x(const float x){
    _x = x;
}

void LS::y(const float y){
    _y = y;
}

void LS::pos(const float x, const float y){
    this->x(x);
    this->y(y);
}

SDL_Color LS::color(void) const{
    return _color;
}

SDL_Color* LS::color_ptr(void){
    return &_color;
}

void LS::color(SDL_Color color){
    _color = color;
}

std::shared_ptr<light::LightImage> LS::get_image(void) const{
    return _image;
}

void LS::set_image(std::shared_ptr<light::LightImage> image){
    _image = image;
}

bool LS::is_on(void) const{
    return _isOn;
}

void LS::active(void){
    _isOn = true;
}

void LS::desactive(void){
    _isOn = false;
}

float LS::angle(void){
    return _angle;
}

void LS::angle(const float angle){
    _angle = angle;
}

LS::Shadow_calculation LS::get_calculation(void) const{
    return calculation_type;
}

std::shared_ptr<light::LightImage> LS::get_calculation_image(void) const{
    return _calculation_image;
}

void LS::set_calculation_image(std::shared_ptr<light::LightImage> image){
    _calculation_image = image;
}

void LS::set_calculation(Shadow_calculation type){
    calculation_type = type;
}