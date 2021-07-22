#include "events/Mouse.hpp"

using M = event::Mouse;

M::Mouse(){
    std::cout << "INFO :: allocating Mouse instance" << std::endl;
    _btn = {false, false, false};
    update();
}

M::~Mouse(){
    std::cout << "INFO :: releasing memory from a Mouse instance" << std::endl;
}

M::MouseButton M::up(void){
    return _up;
}

M::MouseButton M::btn(void){
    return _btn;
}

M::MouseButton M::down(void){
    return _down;
}

void M::update(void){
    _up = {false, false, false};
    _down = {false, false, false};
    _wheel_down = false;
    _wheel_up = false;
}

void M::event(SDL_Event* e){
    switch (e->type){
        case SDL_MOUSEBUTTONDOWN:
            switch (e->button.button){
                case SDL_BUTTON_LEFT:
                    _down.left = true;
                    _btn.left = true;
                    break;
                
                case SDL_BUTTON_RIGHT:
                    _down.right = true;
                    _btn.right = true;
                    break;
                
                case SDL_BUTTON_MIDDLE:
                    _down.middle = true;
                    _btn.middle = true;
                    break;
                
                default:
                    break;
            }
            break;
        
        case SDL_MOUSEBUTTONUP:
            switch (e->button.button){
                case SDL_BUTTON_LEFT:
                    _up.left = true;
                    _btn.left = false;
                    break;
                
                case SDL_BUTTON_MIDDLE:
                    _up.middle = true;
                    _btn.middle = false;
                    break;
                
                case SDL_BUTTON_RIGHT:
                    _up.right = true;
                    _btn.right = false;
                    break;
                
                default:
                    break;
            }
            break;
        
        case SDL_MOUSEMOTION:
            _x = e->motion.x;
            _y = e->motion.y;
            break;
        
        case SDL_MOUSEWHEEL:
            if(e->wheel.y > 0){
                _wheel_up = true;
            } else if(e->wheel.y < 0){
                _wheel_down = true;
            }
        
        default:
            break;
    }
}

int M::x(void){
    return _x;
}

int M::y(void){
    return _y;
}

SDL_Point M::pos(void){
    return {_x, _y};
}

void M::pos(int* x, int* y){
    *x = _x;
    *y = _y;
}

bool M::wheel_up(void){
    return _wheel_up;
}

bool M::wheel_down(void){
    return _wheel_down;
}