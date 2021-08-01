#include "weapons/Projectile.hpp"
#include <cmath>

#define ERRS

#ifdef ERRS
    #define ERR(msg) std::cerr << "ERROR :: " << __func__ << " : " << msg << std::endl;
#else
    #define ERR(msg)
#endif

Projectile::Projectile(){
    _type = nullptr;
    dir_x = 0;
    dir_y = 0;
    owner = nullptr;
}

Projectile::~Projectile(){
    
}

void Projectile::set_type(std::shared_ptr<Projectile_type> type){
    _type = type;
    update_dir();
}

void Projectile::draw(GPU_Target *t, float x, float y, float zoom){
    if (!_type){
        ERR("cannot draw a non linked projectile");
        return;
    }

    GPU_BlitTransform(_type->get_image(), nullptr, t, this->x * zoom - x , this->y * zoom - y, angle + 90, zoom, zoom);
}

bool Projectile::OnTick(const int delta, std::shared_ptr<world::Collisions> collisions){
    
    if (x > 1024 || x < 0) return false;
    if (y > 1024 || y < 0) return false;

    if (dir_x > 0.0f){
        if (collisions->get_dist(x, y, world::Collisions::dir_east, dir_x * delta * 1.2, world::Collisions::Collision_det_player_shoot) >= dir_x * delta){
            x += dir_x * delta;
        } else {
            return false;
        }
    } else {
        if (collisions->get_dist(x, y, world::Collisions::dir_west, std::abs(dir_x * delta * 1.2), world::Collisions::Collision_det_player_shoot) >= std::abs(dir_x * delta)){
           x += dir_x * delta;
        } else {
            return false;
        }
    }

    if (dir_y > 0.0f){
        if (collisions->get_dist(x, y, world::Collisions::dir_south, dir_y * delta * 1.2, world::Collisions::Collision_det_player_shoot) >= dir_y * delta){
            y += dir_y * delta;
        } else {
            return false;
        }
    } else {
        if (collisions->get_dist(x, y, world::Collisions::dir_north, std::abs(dir_y * delta * 1.2), world::Collisions::Collision_det_player_shoot) >= std::abs(dir_y * delta)){
           y += dir_y * delta;
        } else {
            return false;
        }
    }

    return true;
}

void Projectile::set_angle(const float angle){
    this->angle = angle;
    update_dir();
}

void Projectile::update_dir(void){
    const float speed = _type->get_rand_speed();
    dir_x = cos(angle / 180 * M_PI) * speed;
    dir_y = sin(angle / 180 * M_PI) * speed;
}

void Projectile::set_owner(std::shared_ptr<entity::Entity> owner){
    this->owner = owner;
}

std::shared_ptr<entity::Entity> Projectile::get_owner(void){
    return owner;
}

void Projectile::set_x(const float x){
    this->x = x;
}

void Projectile::set_y(const float y){
    this->y = y;
}