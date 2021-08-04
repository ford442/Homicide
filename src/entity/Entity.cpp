#include "entity/Entity.hpp"
#include <cstring>
#include <cmath>
#include "xml.hpp"
#include "dir.hpp"
#include <cassert>

#include "world/World.hpp"

using E = entity::Entity;

E::Entity(std::shared_ptr<sprite::Animations> animations, std::shared_ptr<world::LightSurface> light_surface, float *x, float *y, float *pixel_size, std::shared_ptr<world::Collisions> collisions) : _animations(animations), _light_surface(light_surface), _cam_x(x), _cam_y(y), _pixel_size(pixel_size), Sprite(animations), _collisions(collisions){
    std::cout << "INFO :: allocating Entity instance" << std::endl;
    _collisions_radius = 17;
    reset_animations();
}

E::~Entity(){
    std::cout << "INFO :: releasing memory from a Entity instance" << std::endl;
    reset_animations();
    _lights.clear();
}

inline bool load_animation(XMLNode* node, std::shared_ptr<sprite::SpriteSheet>* animation, int* ticks, std::shared_ptr<sprite::Animations> sources){
    std::cout << "INFO :: load entity animation from \"" << node->tag << "\"" << std::endl;
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (!strcmp(attr.key, "type")){
            std::cout << "INFO :: set animation from \"" << attr.value << "\" animation type" << std::endl;
            *animation = sources->get_sprite(attr.value);

            if (!animation){
                std::cerr << "ERROR :: cannot found \"" << attr.value << "\" sprite type name" << std::endl;
                return false;
            }
            
        } else if (!strcmp(attr.key, "id")){
            std::cout << "INFO :: set animation from \"" << attr.value << "\" animation id" << std::endl;
            int id;
            sscanf(attr.value, "%d", &id);
            *animation = sources->get_sprite(id);

            if (!animation){
                std::cerr << "ERROR :: cannot found \"" << id << "\" sprite type id" << std::endl;
                return false;
            }

        } else if (!strcmp(attr.key, "ticks")){
            if (ticks) sscanf(attr.value, "%d", ticks);

        } else {
            std::cerr << "WARNING :: cannot reconize \"" << attr.key << "\" entity sprite attribute" << std::endl;
        }
    }
    
    return true;
}

bool E::load(XMLNode* node){

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

    }

    for (int c=0; c<node->children.size; c++){
        XMLNode* child = XMLNode_child(node, c);

        if (!strcmp(child->tag, "legs_walk")){
            if (!load_animation(child, &_legs_walk, &_legs_walk_ticks, _animations)) return false;

        } else if (!strcmp(child->tag, "legs_run")){
            if (!load_animation(child, &_legs_run, &_legs_run_ticks, _animations)) return false;

        } else if (!strcmp(child->tag, "legs_left")){
            if (!load_animation(child, &_legs_left, &_legs_left_ticks, _animations)) return false;

        } else if (!strcmp(child->tag, "legs_right")){
            if (!load_animation(child, &_legs_right, &_legs_right_ticks, _animations)) return false;

        } else if (!strcmp(child->tag, "legs_idle")){
            if (!load_animation(child, &_legs_idle, &_legs_idle_ticks, _animations)) return false;

        } else if (!strcmp(child->tag, "body_walk")){
            if (!load_animation(child, &_body_walk, &_body_walk_ticks, _animations)) return false;

        } else if (!strcmp(child->tag, "body_run")){
            if (!load_animation(child, &_body_run, &_body_run_ticks, _animations)) return false;

        } else if (!strcmp(child->tag, "body_idle")){
            if (!load_animation(child, &_body_idle, &_body_idle_ticks, _animations)) return false;

        } else if (!strcmp(child->tag, "collisions")){
            for (int a=0; a<child->attributes.size; a++){
                XMLAttribute attr = child->attributes.data[a];

                if (!strcmp(attr.key, "radius")){
                    sscanf(attr.value, "%d", &_collisions_radius);
                } else {
                    std::cerr << "WARNING :: cannot reconize \"" << attr.key << "\" " << node->tag << " child" << std::endl;
                }
            }
        }
    }

    return true;
}

std::shared_ptr<sprite::SpriteSheet> E::get_legs_walk(int *tick){
   if (tick) *tick = _legs_walk_ticks;
    return _legs_walk;
}

std::shared_ptr<sprite::SpriteSheet> E::get_legs_run(int *tick){
    if (tick) *tick = _legs_run_ticks;
    return _legs_run;
}

std::shared_ptr<sprite::SpriteSheet> E::get_legs_left(int *tick){
   if (tick) *tick = _legs_left_ticks;
    return _legs_left;
}

std::shared_ptr<sprite::SpriteSheet> E::get_legs_right(int *tick){
   if (tick) *tick = _legs_left_ticks;
    return _legs_right;
}

std::shared_ptr<sprite::SpriteSheet> E::get_body_idle(int *tick){
   if (tick) *tick = _body_idle_ticks;
    return _body_idle;
}

std::shared_ptr<sprite::SpriteSheet> E::get_body_run(int *tick){
   if (tick) *tick = _body_run_ticks;
    return _body_run;
}

std::shared_ptr<sprite::SpriteSheet> E::get_body_walk(int *tick){
   if (tick) *tick = _body_walk_ticks;
    return _body_walk;
}

void E::set_x(const float x){
    _x = x;
}

void E::set_y(const float y){
    _y = y;
}

void E::set_location(const float x, const float y){
    set_x(x);
    set_y(y);
}

float E::get_x(void) const{
    return _x * *_pixel_size;
}

float E::get_y(void) const{
    return _y * *_pixel_size;
}

void E::get_location(float *x, float *y) const{
    *x = get_x();
    *y = get_y();
}

float *E::get_x_ptr(void){
    return &_x;
}

float *E::get_y_ptr(void){
    return &_y;
}

void E::get_location_ptr(float *x, float *y){
    x = get_x_ptr();
    y = get_y_ptr();
}

void E::set_speed(const float speed){
    _speed = speed;
}

float E::get_speed(void) const{
    return _speed;
}

float *E::get_speed_ptr(void){
    return &_speed;
}

void E::set_facing_angle(const float angle){
    _facing_angle = angle;
}

float E::get_facing_angle(void) const{
    return _facing_angle;
}

float *E::get_facing_angle_ptr(void){
    return &_facing_angle;
}

void E::OnMouvementUpdate(const float delta){
    if (_speed != 0){
        float x = cos((_moving_angle + _facing_angle) * M_PI / 180) * _speed * delta;
        float y = sin((_moving_angle + _facing_angle) * M_PI / 180) * _speed * delta;

        if (x > 0){
            if (_collisions->get_dist(_x, _y, world::Collisions::dir_east, _collisions_radius, world::Collisions::Collision_all) >= _collisions_radius){
                _x += x;
            }
        } else {
            if (_collisions->get_dist(_x, _y, world::Collisions::dir_west, _collisions_radius, world::Collisions::Collision_all) >= _collisions_radius){
                _x += x;
            }
        }

        if (y > 0){
            if (_collisions->get_dist(_x, _y, world::Collisions::dir_south, _collisions_radius, world::Collisions::Collision_all) >= _collisions_radius){
                _y += y;
            }
        } else {
            if (_collisions->get_dist(_x, _y, world::Collisions::dir_north, _collisions_radius, world::Collisions::Collision_all) >= _collisions_radius){
                _y += y;
            }
        }
    }
}

void E::OnTick(const float delta){
    Sprite::OnTick(delta);
}

void E::OnDraw(GPU_Target* t){
    set_pos(_x, _y);

    auto weapon = get_selected();
    if (weapon){
        GPU_Blit(weapon->get_image(), nullptr, t, _x, _y);
    }

    Sprite::OnDraw(t);

    #ifdef DEBUG_COLLISIONS
        int n = _collisions->get_dist(_x, _y, world::Collisions::dir_north, _collisions_radius, world::Collisions::Collision_all);
        int s = _collisions->get_dist(_x, _y, world::Collisions::dir_south, _collisions_radius, world::Collisions::Collision_all);
        int e = _collisions->get_dist(_x, _y, world::Collisions::dir_east, _collisions_radius, world::Collisions::Collision_all);
        int w = _collisions->get_dist(_x, _y, world::Collisions::dir_west, _collisions_radius, world::Collisions::Collision_all);

        GPU_Line(t, _x, _y, _x, _y - n, {255, 255, 255, 255});
        GPU_Line(t, _x, _y, _x, _y + s, {255, 255, 255, 255});
        GPU_Line(t, _x, _y, _x + e, _y, {255, 255, 255, 255});
        GPU_Line(t, _x, _y, _x - w, _y, {255, 255, 255, 255});
    #endif

}


void E::OnMouseClick(int mouse_btn){

}

void E::OnMouseMovement(const int x, const int y){
    
}

void E::set_animation(const E::EntityAnimation animation){
    _animation = animation;
}

E::EntityAnimation E::get_animation(void) const{
    return _animation;
}

E::EntityAnimation *E::get_animation_ptr(void){
    return &_animation;
}


void E::OnAnimationUpdate(const float delta){
    set_angle(_facing_angle);
}

void E::set_moving_angle(const float angle){
    _moving_angle = angle;
}

float E::get_moving_angle(void) const{
    return _moving_angle;
}

float *E::get_moving_angle_ptr(void){
    return &_moving_angle;
}

void E::reset_animations(void){
    _legs_idle = nullptr;
    _legs_walk = nullptr;
    _legs_left = nullptr;
    _legs_right = nullptr;
    _body_walk = nullptr;
    _body_idle = nullptr;
    _body_run = nullptr;
}

void E::push_light(std::shared_ptr<light::LightSource> light){
    _lights.push_back(light);
}

std::shared_ptr<light::LightSource> E::push_light(std::string light_type){
    std::cout << "INFO :: push light of an entity" << std::endl;
    std::shared_ptr<light::LightSource> light = _light_surface->push_light(light_type);
    std::cout << "INFO :: light pushed into the world light vector" << std::endl;
    if (light != nullptr) push_light(light);
    return light;
}

std::vector<std::shared_ptr<light::LightSource>> *E::get_lights(void){
    return &_lights;
}

void E::update_direction(void){
    _direction.x = cosf(_facing_angle * M_PI / 180);
    _direction.y = sinf(_facing_angle * M_PI / 180);
}

SDL_FPoint E::get_direction(void){
    return _direction;
}

void E::set_direction(float x, float y){
    _direction.x = x;
    _direction.y = y;
}

void E::set_direction(SDL_FPoint dir){
    _direction = dir;
}

SDL_FPoint *E::get_direction_ptr(void){
    return &_direction;
}

std::shared_ptr<world::Collisions> E::get_collisions(void) const{
    return _collisions;
}

void E::shoot(void){
    shooted = true;
    update_direction();
    float x1 = get_x() / *_pixel_size;
    float y1 = get_y() / *_pixel_size;

    float x2 = _direction.x * 1000;
    float y2 = _direction.y * 1000;

    
    std::cout << "INFO :: entity : shoot, x1 : " << x1 << ", y1 : " << y1 << ", x2 : " << x2 << ", y2 : " << y2 << std::endl;

    float m;

    if (x2 != x1){
        m = (y2 - y1) / (x2 - x1);
    }

    int increment = 1;

    if (x2 != x1 && std::abs(m) <= 1.0f){
        
        if (x1 > x2){
            increment = -1;
        }

        const float m = (y2 - y1) / (x2 - x1);
        const float b = y1 - m * x1;

        if (increment == 1){
            for (int x = (int)x1; x<(int)x2; x++){
                const float y = m * (float)x + b;
                if (_collisions->is_shoot_collision(_collisions->get(x, y))){
                    shoot_contact = {float(x), float(y)};
                    return;     
                }
            }
        } else {
            for (int x = (int)x1; x>(int)x2; x--){
                const float y = m * (float)x + b;
                if (_collisions->is_shoot_collision(_collisions->get(x, y))){
                    shoot_contact = {float(x), float(y)};
                    return;     
                }
            }
        }
    } else {
        if (y1 > y2){
            increment = -1;
        }
        const float w = (x2 - x1) / (y2 - y1);
        const float p = x1 - w * y1;

        if (increment == 1){
            for (int y = (int)y1; y<(int)y2; y++){
                const float x = w * (float)y + p;
                if (_collisions->is_shoot_collision(_collisions->get(x, y))){
                    shoot_contact = {float(x), float(y)};
                    return;     
                }
            }
        } else {
            for (int y = (int)y1; y>(int)y2; y--){
                const float x = w * (float)y + p;
                if (_collisions->is_shoot_collision(_collisions->get(x, y))){
                    shoot_contact = {float(x), float(y)};
                    return;
                }
            }
        }
    }

    shoot_contact = {x2, y2};
    return;
}


void E::OnLightDraw(GPU_Target *t){
    if (shooted){
        GPU_Line(t, get_x() / *_pixel_size, get_y() / *_pixel_size, shoot_contact.x, shoot_contact.y, {255, 255, 255, 255});
        shooted = false;
    }
}

float E:: get_facing(void) const{
    return _facing_angle;
}