#include "events/Event_handler.hpp"

using H = event::Handler;

H::Handler(){
    std::cout << "INFO :: allocating EventHandler" << std::endl;
    _mouse = std::make_unique<event::Mouse>();
    _keypad = std::make_unique<event::Keypad>();
}

H::~Handler(){
    std::cout << "INFO :: releasing memory from a EventHandler" << std::endl;
    _mouse = nullptr;
    _keypad = nullptr;
}

void H::event(SDL_Event* e){
    _mouse->event(e);
    _keypad->event(e);
}

void H::update(void){
    _keypad->update();
    _mouse->update();
}

bool H::IsKeyDown(SDL_Scancode key){
    return _keypad->key(key);
}

bool H::IsKeyPush(SDL_Scancode key){
    return _keypad->down(key);
}

bool H::IsKeyRelease(SDL_Scancode key){
    return _keypad->up(key);
}

bool H::isButtonDown(event::MouseButtonEnum btn){
    switch (btn){
        case Mouse_button_left:
            return _mouse->btn().left;
        
        case Mouse_button_middle:
            return _mouse->btn().middle;
        
        case Mouse_button_right:
            return _mouse->btn().right;
        
        default:
            return false;
    }
}

bool H::isButtonPush(event::MouseButtonEnum btn){
    switch (btn){
        case Mouse_button_left:
            return _mouse->down().left;
        
        case Mouse_button_middle:
            return _mouse->down().middle;
        
        case Mouse_button_right:
            return _mouse->down().right;
        
        default:
            return false;
    }
}

bool H::isButtonRelease(event::MouseButtonEnum btn){
    switch (btn){
        case Mouse_button_left:
            return _mouse->up().left;
        
        case Mouse_button_middle:
            return _mouse->up().middle;
        
        case Mouse_button_right:
            return _mouse->up().right;
        
        default:
            return false;
    }
}

int H::mouse_x(void) const{
    return _mouse->x();
}

int H::mouse_y(void) const{
    return _mouse->y();
}

void H::mouse_pos(int *x, int *y){
    *x = mouse_x();
    *y = mouse_y();
}

bool H::is_wheel_up(void) const{
    return _mouse->wheel_up();
}

bool H::is_wheel_down(void) const{
    return _mouse->wheel_down();
}