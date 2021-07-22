#include "sprites/Sprite.hpp"

using S = sprite::Sprite;

S::Sprite(std::shared_ptr<sprite::Animations> animations) : _animations(animations){
    std::cout << "INFO :: allocating a Sprite instance" << std::endl;
    _id = 0;
    _spriteSheet = nullptr;
    _ticks = SDL_GetTicks();
}

S::~Sprite(){
    std::cout << "INFO :: releasing memory from a Sprite instance" << std::endl;
    _spriteSheet = nullptr;
    _animations = nullptr;
}

void S::set_delay(const int delay){
    _delay = delay;
}

int S::get_delay(void) const{
    return _delay;
}

int *S::get_delay_ptr(void){
    return &_delay;
}

void S::set_id(const int id){
    _id = id;
}

int S::get_id(void) const{
    return _id;
}

void S::OnTick(const int delta){
    if (!_pause){
        if (_spriteSheet == nullptr) return;

        _ticks += delta;

        while (_ticks > _delay){
            _id ++;
            _ticks -= _delay;
        }
    }
}

void S::OnDraw(GPU_Target* t){
    if (_spriteSheet == nullptr){
        std::cerr << "WARNING :: cannot draw a onlinked sprite : src spritsheet : " << _spriteSheet << std::endl;
        return;
    }

    if (_angle == 0){
        _spriteSheet->draw(t, nullptr, {_x, _y}, _id);
    } else {
        _spriteSheet->drawRotate(t, nullptr, {_x, _y}, _angle, _id);
    }
}

void S::set_pos(const float x, const float y){
    set_x(x);
    set_y(y);
}

void S::set_x(const float x){
    _x = x;
}

void S::set_y(const float y){
    _y = y;
}

void S::set_angle(const float angle){
    _angle = angle;
}

float *S::get_x_ptr(void){
    return &_x;
}

float *S::get_y_ptr(void){
    return &_y;
}

void S::get_pos_ptr(float *x, float *y){
    x = get_x_ptr();
    y = get_y_ptr();
}

float *S::get_angle_ptr(void){
    return &_angle;
}

void S::pause(void){
    _pause = true;
}

void S::play(void){
    _pause = false;
}

void S::set_spriteSheet(std::shared_ptr<sprite::SpriteSheet> spriteSheet){
    _spriteSheet = spriteSheet;
}

bool S::load_spriteSheet(std::string sprite_type){
    _spriteSheet = _animations->get_sprite(sprite_type);
    return _spriteSheet != nullptr;
}

std::shared_ptr<sprite::SpriteSheet> S::get_spriteSheet(void) const{
    return _spriteSheet;
}