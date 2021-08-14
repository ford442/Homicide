#include "events/Keypad.hpp"

using K = event::Keypad;

K::Keypad(){
    #ifdef KEY_LOGS
        std::cout << "INFO :: allocating Keypad instance" << std::endl;
    #endif
    for (int i=0; i<UINT8_MAX; i++){
        _down[i] = false;
        _up[i] = true;
        _key[i] = false;
    }
}

K::~Keypad(){
    #ifdef KEY_LOGS
        std::cout << "INFO :: releasing memory from a Keypad instance" << std::endl;
    #endif
}

void K::update(void){
    for (int i=0; i<UINT8_MAX; i++){
        _down[i] = false;
        _up[i] = false;
    }
}

void K::event(SDL_Event* e){
    switch (e->type){
        case SDL_KEYDOWN:
            #ifdef KEY_KEYDOWN
                std::cout << "INFO :: event : keydown \'" << SDL_GetScancodeName(e->key.keysym.scancode) << "\'" << std::endl;
            #endif

            _down[e->key.keysym.scancode] = true;
            _key[e->key.keysym.scancode] = true;
            break;
        
        case SDL_KEYUP:
            #ifdef KEY_KEYUP
                std::cout << "INFO :: event : keyup \'" << SDL_GetScancodeName(e->key.keysym.scancode) << "\'" << std::endl;
            #endif

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