#include "lights/IlluminatedImage.hpp"
#include "dir.hpp"
#include <SDL2/SDL_image.h>

using I = light::IlluminatedImage;

I::IlluminatedImage(void){
    std::cout << "INFO :: allocating a IlluminatedImage instance" << std::endl;
    clear_members();
}

I::~IlluminatedImage(){
    std::cout << "INFO :: releasing memory from a IlluminatedImage instance" << std::endl;
    clear_members();
}

GPU_Image* I::get_image(void){
    return _image;
}

void I::set_image(GPU_Image* image){
    _image = image;
}

bool I::load(std::string path){
    if (path[1] != ':') path = RES + path;
    SDL_Surface *surface = IMG_Load(path.c_str());

    if (!surface){
        std::cerr << "ERROR :: IMG_Load : " << IMG_GetError << ", file : " << path << std::endl;
        return false;
    }

    _image = GPU_CopyImageFromSurface(surface);
    SDL_FreeSurface(surface);
    return true;
}

void I::blit(GPU_Target* t){
    GPU_BlitTransformX(_image, nullptr, t, _x, _y, _cx, _cy, _angle, _scale, _scale);
}


void I::set_x(float x){
    _x = x;
}

void I::set_y(float y){
    _y = y;
}

void I::set_pos(float x, float y){
    set_x(x);
    set_y(y);
}

float I::get_x(void){
    return _x;
}

float I::get_y(void){
    return _y;
}

float *I::get_x_ptr(void){
    return &_x;
}

float *I::get_y_ptr(void){
    return &_y;
}

void I::get_pos(float *x, float *y){
    *x = get_x();
    *y = get_y();
}

void I::get_pos_ptr(float *x, float *y){
    x = get_x_ptr();
    y = get_y_ptr();
}

void I::set_angle(float angle){
    _angle = angle;
}

float I::get_angle(void){
    return _angle;
}

float *I::get_angle_ptr(void){
    return &_angle;
}

void I::set_center_x(float x){
    _cx = x;
}

void I::set_center_y(float y){
    _cy = y;
}

void I::set_center_pos(float x, float y){
    set_center_x(x);
    set_center_y(y);
}

float I::get_center_x(void){
    return _cx;
}

float I::get_center_y(void){
    return _cy;
}

float *I::get_center_x_ptr(void){
    return &_cx;
}

float *I::get_center_y_ptr(void){
    return &_cy;
}

void I::get_center_pos(float *x, float *y){
    *x = get_center_x();
    *y = get_center_y();
}

void I::get_center_pos_ptr(float *x, float *y){
    x = get_center_x_ptr();
    y = get_center_y_ptr();
}

void I::set_scale(float scale){
    _scale = scale;
}

float I::get_scale(void){
    return _scale;
}

float *I::get_scale_ptr(void){
    return &_scale;
}

void I::clear_members(void){
    set_x(0);
    set_y(0);
    set_angle(0);
    set_center_x(0);
    set_center_y(0);
    set_scale(1);
    set_image(nullptr);
}

void I::recenter(void){
    if (_image){
        set_center_x(_image->w / 2);
        set_center_y(_image->h / 2);
    } else {
        set_center_x(0);
        set_center_y(0);
    }
}