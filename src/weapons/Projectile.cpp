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
    light = nullptr;
}

Projectile::~Projectile(){
    
}

void Projectile::set_type(std::shared_ptr<Projectile_type> type){
    _type = type;
    update_dir();

    light = std::make_shared<light::LightSource>();
    
    // light->set_image(type->get_light_image());
    light->set_calculation_image(type->get_light_image());
    light->set_calculation(light::LightSource::Shadow_image);
    light->x(x);
    light->y(y);
}

void Projectile::draw(GPU_Target *t){
    if (!_type){
        ERR("cannot draw a non linked projectile");
        return;
    }

    GPU_BlitRotate(_type->get_image(), nullptr, t, x, y, angle);
}

void Projectile::OnTick(const int delta){
    x += dir_x * delta;
    y += dir_y * delta;
    light->x(x);
    light->y(y);
}

void Projectile::set_angle(const float angle){
    this->angle = angle;
    light->angle(angle);
    update_dir();
}

void Projectile::update_dir(void){
    dir_x = cos(angle / 180 * M_PI) * _type->get_speed();
    dir_y = sin(angle / 180 * M_PI) * _type->get_speed();
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