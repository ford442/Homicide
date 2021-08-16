#include "events/Keypad.hpp"

using K = event::Keypad;

K::Keypad(){
    for (int i=0; i<SDL_NUM_SCANCODES; i++){
        _down[i] = false;
        _up[i] = true;
        _key[i] = false;
    }
}

K::~Keypad(){
}

void K::update(void){
    for (int i=0; i<SDL_NUM_SCANCODES; i++){
        _down[i] = false;
        _up[i] = false;
    }
}

void K::event(SDL_Event* e){
    switch (e->type){
        case SDL_KEYDOWN:
            _down[e->key.keysym.scancode] = true;
            _key[e->key.keysym.scancode] = true;
            break;
        
        case SDL_KEYUP:
            _up[e->key.keysym.scancode] = true;
            _key[e->key.keysym.scancode] = false;
            break;
        
        default:
            break;
    }
}

bool K::up(SDL_Scancode code){
    return _up[code];
}

bool K::down(SDL_Scancode code){
    return _down[code];
} 

bool K::key(SDL_Scancode code){
    return _key[code];
}