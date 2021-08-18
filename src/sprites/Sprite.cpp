#include "sprites/Sprite.hpp"
#include "main.hpp"

Sprite::Sprite(){
    spriteSheet = nullptr;
    _scale = 1;
    _id = 0;
    _ticks = 0;
    _pause = false;
    _delay = 1;
}

Sprite::~Sprite(){

}

void Sprite::set_delay(const int delay){
    _delay = delay;
}

int Sprite::get_delay(void) const{
    return _delay;
}

void Sprite::set_id(const int id){
    _id = id;
}

int Sprite::get_id(void) const{
    return _id;
}

void Sprite::OnTick(const int delta){
    if (!is_paused()){
        _ticks += delta;

        while (_ticks >= _delay){
            _ticks -= _delay;
            _id ++;
        }

        if (spriteSheet != nullptr){
            while (spriteSheet->size() <= _id){
                _id -= spriteSheet->size();
            }
        }
    }
}

void Sprite::OnDraw(GPU_Target* t, const float x, const float y, const float zoom){
    if (spriteSheet == nullptr) return;

    std::cout << _id << std::endl;
    GPU_Image *image = spriteSheet->get_image(_id);
    if (!image) return;
    GPU_BlitTransform(image, nullptr, t, _x * zoom - x, _y * zoom - y, _angle, _scale * zoom, _scale * zoom);
}

std::shared_ptr<sprite::SpriteSheet> get_sprite(std::string value, std::list<std::shared_ptr<sprite::SpriteSheet>> &sprites){
    for (auto &s : sprites){
        if (s->name() == value) return s;
    }
    return nullptr;
}

bool Sprite::load(XMLNode *node, std::list<std::shared_ptr<sprite::SpriteSheet>> &sprites){
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "spriteSheet")){
            spriteSheet = get_sprite(attr.value, sprites);

            if (spriteSheet != nullptr){
                sprite_sheet_name = attr.value;
            } else {
                ERR("cannot found \"" + std::string(attr.value) + "\" spritesheet");
                return false;
            }
        } else if (is_equal(attr.key, "delay")){
            try {
                _delay = std::stoi(attr.value);
            } catch (std::exception &e){
                ERR("standart excpetion : "  + std::string(e.what()));
            }

            if (_delay <= 0){
                WARN("cannot set the delay at a negativ or nul value : " + std::to_string(_delay) + " <= 0");
                _delay = 1;
            }
            
        } else if (is_equal(attr.key, "id")){
            try {
                _id = std::stoi(attr.value);
            } catch (std::exception &e){
                ERR("standart excpetion : "  + std::string(e.what()));
            }
        } else if (is_equal(attr.key, "scale")){
            try {
                _scale = std::stof(attr.value);
            } catch (std::exception &e){
                ERR("standart excpetion : "  + std::string(e.what()));
            }

            if (_scale < 0){
                WARN("cannot set the scale of the sprite at a negativ value : " + std::to_string(_scale) + " < 0");
                _scale = 0.1;
            }

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" sprite attribute");
        }
    }

    if (sprite_sheet_name.empty()){
        ERR("cannot load a sprite without a valid spriteSheet");
        return false;
    }

    if (_id > spriteSheet->size())
        _id = spriteSheet->size();
    
    return true;
}

void Sprite::pos(const float x, const float y){
    this->x(x);
    this->y(y);
}

void Sprite::x(const float x){
    _x = x;
}

void Sprite::y(const float y){
    _y = y;
}

const float Sprite::x(void) const{
    return _x;
}

const float Sprite::y(void) const{
    return _y;
}

void Sprite::pos(float *x, float *y) const{
    *x = this->x();
    *y = this->y();
}

void Sprite::set_angle(const float angle){
    _angle = angle;
}

bool Sprite::is_paused(void) const{
    return _pause;
}

void Sprite::pause(void){
    _pause = true;
}

void Sprite::play(void){
    _pause = false;
}

void Sprite::set_spriteSheet(std::shared_ptr<sprite::SpriteSheet> spriteSheet){
    this->spriteSheet = spriteSheet;
}

std::shared_ptr<sprite::SpriteSheet> Sprite::get_spriteSheet(void) const{
    return spriteSheet;
}