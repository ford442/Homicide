#include "entity/Entity.hpp"
#include <cstring>
#include <cmath>
#include "main.hpp"
#include "dir.hpp"

Entity::Entity() : Sprite(), ShadowBlock(){
    collisions = nullptr;
    pos(0.0f, 0.0f);
    facing(0.0f);
    direction(0.0f);
}

Entity::~Entity(){

}

void Entity::OnTick(const int delta){
    Sprite::pos(x(), y());
    ShadowBlock::pos(x(), y());

    Sprite::set_angle(facing());
    ShadowBlock::angle(facing());

    ShadowBlock::OnTick();
    Sprite::OnTick(delta);
}

void Entity::OnDraw(GPU_Target *t, const float x, const float y, const float zoom){
    GPU_Circle(t, _x * zoom - x , _y * zoom - y, 40 * zoom, {255, 0, 0, 255});
    Sprite::OnDraw(t, x, y, zoom);
}   

bool Entity::load(XMLNode *node, std::list<std::shared_ptr<sprite::SpriteSheet>> &sprites, ShadowCaster *shadowCaster){
    if (!collisions){
        ERR("cannot load an entity whithout a valid collision");
        return false;
    }
    
    if (!load_xml_attribute(node)) return false;
    
    for (int c=0; c<node->children.size; c++){
        XMLNode *child = XMLNode_child(node, c);

        if (is_equal(child->tag, "sprite")){
            if (!Sprite::load(child, sprites)) return false;
        
        } else if (is_equal(child->tag, "shadow")){
            if (!ShadowBlock::load(child, shadowCaster)) return false;

        }

    }

    return true;
}

bool Entity::is_running(void){
    return running;
}

void Entity::run(void){
    running = true;
}

void Entity::walk(void){
    running = false;
}

const float Entity::x(void) const{
    return _x;
}

const float Entity::y(void) const{
    return _y;
}

void Entity::pos(float *x, float *y){
    *x = this->x();
    *y = this->y();
}

void Entity::x(const float x){
    _x = x;
}

void Entity::y(const float y){
    _y = y;
}

void Entity::pos(const float x, const float y){
    this->x(x);
    this->y(y);
}

const float Entity::facing(void) const{
    return _facing_angle;
}

void Entity::facing(const float facing){
    _facing_angle = facing;
}

const float Entity::direction(void) const{
    return _direction;
}

void Entity::direction(const float direction){
    _direction = direction;
}

const float Entity::run_speed(void) const{
    return _run_speed;
}

void Entity::run_speed(const float speed){
    _run_speed = speed;
}

const float Entity::walk_speed(void) const{
    return _walk_speed;
}

void Entity::walk_speed(const float speed){
    _walk_speed = speed;
}

const int Entity::str_to_int(std::string str){
    int value = 0;

    try {
        value = std::stoi(str);
    } catch (std::exception &e){
        ERR("standart excpetion : " + std::string(e.what()));
    }

    return value;
}

const float Entity::str_to_float(std::string str){
    float value = 0;

    try {
        value = std::stof(str);
    } catch (std::exception &e){
        ERR("standart excpetion : " + std::string(e.what()));
    }

    return value;
}

bool Entity::load_xml_attribute(XMLNode *node){

    if (!node->attributes.size){
        ERR("cannot load an entity without any attributes in his node");
        return false;
    }

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "x"))
            x(str_to_float(attr.value));
        
        else if (is_equal(attr.key, "y"))
            y(str_to_float(attr.value));
        
        else if (is_equal(attr.key, "facing"))
            facing(str_to_float(attr.value));
        
        else if (is_equal(attr.key, "direction"))
            direction(str_to_float(attr.value));
    }

    return true;
}

void Entity::set_collisions(world::Collisions *collisions){
    this->collisions = collisions;
}