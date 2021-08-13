#include "lights/LightSource.hpp"
#include "main.hpp"

using namespace light;

LightSource::LightSource(){
    target = nullptr;
    image = nullptr;
    on();
    unlock();
    pos(0, 0);
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
    if (_x != x) moved = true;
    _x = x;
}

void LightSource::y(const int y){
    if (_y != y) moved = true;
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
    if (moved && !locked){
        update_target();
    }
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

    _w = w;
    _h = h;

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

std::vector<ShadowCaster::Visibility_poly_point> LightSource::get_vibility_poly(void) const{
    return visibility_poly;
}

void LightSource::update_target(void){
    GPU_Clear(target);

    if (visibility_poly.size() > 1){

        for (int i=0; i<visibility_poly.size()-1; i++){

            float x1 = visibility_poly[i].x; 
            float y1 = visibility_poly[i].y;
            float x2 = visibility_poly[i+1].x; 
            float y2 = visibility_poly[i+1].y; 

            GPU_TriFilled(target, _x, _y, x1, y1, x2, y2, {255, 255, 255, 255});
        }

        float x1 = visibility_poly[0].x; 
        float y1 = visibility_poly[0].y; 
        float x2 = visibility_poly[visibility_poly.size()-1].x;
        float y2 = visibility_poly[visibility_poly.size()-1].y;

        GPU_TriFilled(target, _x, _y, x1, y1, x2, y2, {255, 255, 255, 255});
    }
    GPU_Flip(target);

}

void LightSource::update_visibility_poly(ShadowCaster* shadowCaster){
    shadowCaster->calculate(_x, _y, visibility_poly, _w, _h);
}

GPU_Target *LightSource::get_target(void) const{
    return target;
}

GPU_Image *LightSource::get_image(void) const{
    return image;
}