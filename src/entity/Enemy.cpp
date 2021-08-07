#include "entity/Enemy.hpp"

#define LOGS_OUTPUT

using E = entity::Enemy;
E::Enemy(std::shared_ptr<sprite::Animations> animations, float *x, float *y, float *pixel_size, std::shared_ptr<world::Collisions> collisions, std::shared_ptr<world::A_star> A_star) : NPC(animations, x, y, pixel_size, collisions, A_star){
    #ifdef LOGS_OUTPUT
        std::cout << "INFO :: Enemy : allocating a new Enemy instance" << std::endl;
    #endif
}

E::~Enemy(){
    #ifdef LOGS_OUTPUT
        std::cout << "INFO :: Enemy : releasing memory from an Entity instance" << std::endl;
    #endif
}

bool E::load(XMLNode* node){
    #ifdef LOGS_OUTPUT
        std::cout << "INFO :: Enemy : load from a xml node" << std::endl;
    #endif
    if (!Entity::load(node)) return false;

    int tick;
    set_spriteSheet(get_body_idle(&tick));
    set_delay(tick);

    set_location(500, 500);
    go_to(rand() % 1000, rand() % 1000);
    walk();

    return true;
}

void E::OnDraw(GPU_Target *t){
    NPC::OnDraw(t);
    set_pos(get_x() / *_pixel_size, get_y() / *_pixel_size);
    Sprite::OnDraw(t);
}

void E::OnTick(const float delta){
    NPC::OnTick(delta);
}

void E::OnDestinationAchieved(void){
    #ifdef LOGS_OUTPUT
        // std::cout << "INFO :: Enemy : destination achieved" << std::endl;
    #endif
    go_to(rand() % 1000, rand() % 1000);
}