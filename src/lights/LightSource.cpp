#include "lights/LightSource.hpp"
#include "main.hpp"

using namespace light;

LightSource::LightSource(){
    target = nullptr;
    image = nullptr;
}

LightSource::~LightSource(){
    destroy_target();
}

const int LightSource::x(void) const{
    return _x;
}

const int LightSource::y(void) const{
    return _y;
}

void LightSource::pos(int *x, int *y) const{
    *x = this->x();
    *y = this->y();
}

void LightSource::x(const int x){
    _x = x;
}

void LightSource::y(const int y){
    _y = y;
}

void LightSource::pos(const int x, const int y){
    this->x(x);
    this->y(y);    
}

void LightSource::on(void){
    active = true;
}

void LightSource::off(void){
    active = false;
}

bool LightSource::is_on(void) const{
    return active;
}

void LightSource::OnTick(void){

}

void LightSource::lock(void){
    locked = true;
}

void LightSource::unlock(void){
    locked = false;
}

bool LightSource::is_locked(void) const{
    return locked;
}

bool LightSource::update_size(const int w, const int h){

    if (w <= 0 || h <= 0){
        ERR("cannot set the size of a light source with a negative or nul width or / and height");
        return false;
    }

    destroy_target();

    GPU_Image *image = GPU_CreateImage(w, h, GPU_FORMAT_LUMINANCE_ALPHA);

    if (!image){
        ERR("GPU_CreateImage");
        return false;
    }

    GPU_Target *target = GPU_LoadTarget(image);
    
    if (!target){
        GPU_FreeImage(image);
        ERR("GPU_LoadTarget");
        return false;
    }

    this->image = image;
    this->target = target;
    return true; 
}

void LightSource::destroy_target(void){
    if (target){
        GPU_FreeTarget(target);
        target = nullptr;
    }

    if (image){
        GPU_FreeImage(image);
        image = nullptr;
    }
}