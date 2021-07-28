#include "entity/NPC.hpp"
#include <cmath>

using N = entity::NPC;

// #define DRAW_PATH
// #define DRAW_TARGET
// #define DRAW_BASE

N::NPC(std::shared_ptr<sprite::Animations> animations, std::shared_ptr<world::LightSurface> light_surface, float *x, float *y, float *pixel_size, std::shared_ptr<world::Collisions> collisions, std::shared_ptr<world::A_star> A_star) : Entity(animations, light_surface, x, y, pixel_size, collisions), _A_star(A_star){
    std::cout << "INFO :: NPC : new NPC instance" << std::endl;
    set_moving_angle(0);
    _target = nullptr;
    walk_speed = 0.1;
    run_speed = 0.3;

    std::cout << "INFO :: NPC : allocating visited bool table : " << A_star->get_w() << " x " << A_star->get_h() << " : " << A_star->get_w() * A_star->get_h() << std::endl;
    _path.table = std::make_unique<world::Path_node[]>(A_star->get_w() * A_star->get_h());
}
    
N::~NPC(){
    std::cout << "INFO :: NPC : releasing NPC instance from memory" << std::endl;
    _path.list.clear();
    _target = nullptr;
}

void N::OnTick(const float delta){
    update_speed();

    update_target_node();

    if (!_path.calculating)
        if (_target && (pow(_target->x * _A_star->get_padding() - (get_x() / *_pixel_size), 2) + pow(_target->y * _A_star->get_padding()  - (get_y() / *_pixel_size), 2) < 100))
            OnTargetAchieved();
        
}

void N::OnDraw(GPU_Target* t){

    #ifdef DRAW_PATH
        for (int i=1; i<_path.list.size(); i++){
            GPU_Line(t, _path.list[i-1]->x * _A_star->get_padding(), _path.list[i-1]->y * _A_star->get_padding(), _path.list[i]->x * _A_star->get_padding(), _path.list[i]->y * _A_star->get_padding(), {255, 255, 255, 255});
        }
    #endif

    #ifdef DRAW_BASE
        const float x = get_x() / *_pixel_size;
        const float y = get_y() / *_pixel_size;

        SDL_Color col = {255, 255, 0, 255};

        if (_path.calculating){
            col = {255, 255, 255, 255};
        }

        GPU_CircleFilled(t, x, y, 15, col);
        GPU_Line(t, x, y, x + (cos(get_facing_angle() * M_PI / 180) * 20), y + (sin(get_facing_angle() * M_PI / 180) * 20), {255, 0, 0, 255});
    #endif

    #ifdef DRAW_TARGET
        if (_target){
            GPU_Circle(t, _target->x * _A_star->get_padding(), _target->y * _A_star->get_padding(), 7, {255, 0, 0, 255});
        }
    #endif

}

void N::go_to(const float x, const float y){
    if (_A_star == nullptr) return;
    if (_path.calculating) return;
    // _A_star->calculate_path(get_x() / *_pixel_size, get_y() / *_pixel_size, x, y, &_path);
    world::calculate_Astar(get_x() / *_pixel_size, get_y() / *_pixel_size, x, y, &_path, _A_star.get());
}


void N::walk(void){
    movement_type = 1;
}

void N::run(void){
    movement_type = 2;
}

void N::stop(void){
    movement_type = 0;
}

void N::update_target_node(void){

    if (!_path.list.empty()){
        _target = _path.list.back();
        set_facing_angle(-(atan2(_target->x * _A_star->get_padding() - get_x() / *_pixel_size, _target->y * _A_star->get_padding() - get_y() / *_pixel_size) * 180 / M_PI) + 90);
    } else {
        _target = nullptr;
        OnDestinationAchieved();
    }
}

void N::update_speed(void){
    if (!_target){
        set_speed(0);

    } else {
        switch (movement_type){
            case 1:
                set_speed(walk_speed);
                break;
            
            case 2:
                set_speed(run_speed);
                break;
            
            default:
                set_speed(0);
                break;
        }
    }
}

void N::set_walk_speed(const float speed){
    walk_speed = speed;
}

void N::set_run_speed(const float speed){
    run_speed = speed;
}

void N::OnTargetAchieved(void){
    _path.list.pop_back();
    update_target_node();
}

void N::OnDestinationAchieved(void){

}
