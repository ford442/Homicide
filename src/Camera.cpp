#include "Camera.hpp"
#include <cmath>

#ifndef M_PI
    #define M_PI 3.141592
#endif

using C = Camera;

C::Camera(void){
    _x = 0;
    _y = 0;
    _ticks = 0;
    go_to(0, 0);
    zoom(0);
}

C::~Camera(){
    
}

void C::setMovementType(C::CameraMovementType type){
    movement_type = type;
}

void C::OnTick(const int delta){
    _ticks += delta;

    if (_ticks != 0)
        _coef = float(_ticks) / float(_delay) / 2;
    else
        _coef = 0;
    
    if (_coef > 0.5) _coef = 0.5;

    if (_moving){
        switch (movement_type){
            case Camera_FadeIn:
                _x = _prev_x + (sin(_coef * M_PI) * _dif_x);
                _y = _prev_y + (sin(_coef * M_PI) * _dif_y);
                _zoom = _prev_zoom + (sin(_coef * M_PI) * _dif_zoom);
                break;
            
            case Camera_FadeOut:
                _x = _prev_x + (cos(_coef * M_PI) * _dif_x);
                _y = _prev_y + (cos(_coef * M_PI) * _dif_y);
                _zoom = _prev_zoom + (cos(_coef * M_PI) * _dif_zoom);
                break;
            
            case Camera_Linear:
                _x = _prev_x + (_coef*2 * _dif_x);
                _y = _prev_y + (_coef*2 * _dif_y);
                _zoom = _prev_zoom + (_coef*2 * _dif_zoom);
                break;
            
            default:
                setMovementType(Camera_Linear);
                break;
        }

        if (_coef == 0.5) _moving = false;
    }
    
}

void C::go_to(float x, float y){
    _prev_x = _x;
    _prev_y = _y;
    _target_x = x;
    _target_y = y;
    update_dif();
}

void C::get_pos(float *x, float *y){
    *x = _x;
    *y = _y;
}

void C::get_zoom(float *zoom){
    *zoom = _zoom;
}

void C::update_dif(void){
    _dif_x = _target_x - _x;
    _dif_y = _target_y - _y;
    _dif_zoom = _zoom - _target_zoom;
    _ticks = 0;
    _moving = true;
}

bool C::should_move(void) const{
    return _x != _target_x || _y != _target_y || _zoom != _target_zoom;
}

void C::zoom(float zoom){
    _prev_zoom = zoom;
    _target_zoom = zoom;
    update_dif();
}

void C::set_delay(int delay){
    _delay = delay;
}

void C::OnDraw(GPU_Target *t){
    GPU_Pixel(t, _x, _y, {255, 255, 255, 255});
}