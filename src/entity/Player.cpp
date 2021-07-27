#include "entity/Player.hpp"
#include <cstring>
#include <cmath>

using P = entity::Player;

P::Player(std::shared_ptr<sprite::Animations> animations, std::shared_ptr<world::LightSurface> lightSurface, std::shared_ptr<event::Handler> events, float *x, float *y, float *pixel_size, std::shared_ptr<world::Collisions> collisions) : Entity(animations, lightSurface, x, y, pixel_size, collisions), _events(events){
    std::cout << "INFO :: allocating a player instance" << std::endl;
    set_forward_key(SDL_SCANCODE_W);
    set_backward_key(SDL_SCANCODE_Z);
    set_left_key(SDL_SCANCODE_A);
    set_right_key(SDL_SCANCODE_D);
    set_run_key(SDL_SCANCODE_LSHIFT);
}

P::~Player(){
    std::cout << "INFO :: releasning memory from a player instance" << std::endl;
}

void P::OnMouseMovement(const int x, const int y){
    update_direction();
    set_facing_angle(-(atan2(x - get_x() + *_cam_x, y - get_y() + *_cam_y) * 180 / M_PI) + 90);
    _light->angle(get_facing_angle() + 90 + cos(light_add_angle)*2);
    _light->pos(get_x() / *_pixel_size, get_y() / *_pixel_size);
}

SDL_Scancode get_key(XMLNode *node){
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (!strcmp(attr.key, "key")){
            return SDL_GetScancodeFromName(attr.value);
        } else {
            std::cerr << "WARNING :: cannot reconize \"" << attr.key << "\" player key attribute in \"" << node->tag << "\" node" << std::endl;
        }
    }
    return SDL_SCANCODE_0;
}

void loadKey(P *p, XMLNode* node){
    std::cout << "INFO :: load player keys" << std::endl;
    for (int c=0; c<node->children.size; c++){
        XMLNode *child = XMLNode_child(node, c);

        if (!strcmp(child->tag, "forward")){
            p->set_forward_key(get_key(child));

        } else if (!strcmp(child->tag, "backward")){
            p->set_backward_key(get_key(child));

        } else if (!strcmp(child->tag, "left")){
            p->set_left_key(get_key(child));

        } else if (!strcmp(child->tag, "right")){
            p->set_right_key(get_key(child));
        
        } else if (!strcmp(child->tag, "run")){
            p->set_run_key(get_key(child));

        } else {
            std::cerr << "WARNING :: cannot reconize \"" << child->tag << "\" player key" << std::endl;
        }
    }
}

void P::load(XMLNode* node){
    std::cout << "INFO :: load the player form a xml node" << std::endl;
    for (int c=0; c<node->children.size; c++){
        XMLNode *child = XMLNode_child(node, c);

        if (!strcmp(child->tag, "key")){
            loadKey(this, child);

        } else if (!strcmp(child->tag, "light")){
            std::cout << "INFO :: set player light" << std::endl;
            for (int a=0; a<child->attributes.size; a++){
                XMLAttribute attr = child->attributes.data[a];

                if (!strcmp(attr.key, "type")){
                    _light = push_light(attr.value);
                    _light->color({255, 255, 255, 255});
                    _light->set_calculation(light::LightSource::Shadow_pylogon);
                } else {
                    std::cerr << "WARNING :: cannot reconize  \"" << attr.key << "\" light player attribute" << std::endl;
                }
            }
        } else if (!strcmp(child->tag, "animations")){
            Entity::load(child);
        
        } else {
            std::cerr << "WARNING :: cannot reconize \"" << child->tag << "\" player child" << std::endl;
        }
    }
}

void P::OnMouseClick(int mouse_btn){

}

void P::set_forward_key(SDL_Scancode key){
    _forward = key;
}

void P::set_backward_key(SDL_Scancode key){
    _backward = key;
}

void P::set_left_key(SDL_Scancode key){
    _left = key;
}

void P::set_right_key(SDL_Scancode key){
    _right = key;
}

void P::OnTick(const float delta){

    bool is_key = false;
    int tick;

    if (light_add_angle > M_PI*2) light_add_angle -= M_PI*2;

    if (_events->IsKeyDown(_backward)){
        set_speed(-0.15);
        set_moving_angle(0);
        is_key = true;
        set_spriteSheet(get_body_walk(&tick));
        set_delay(tick);
        light_add_angle += delta/250;
    }

    if (_events->IsKeyDown(_forward)){
        set_speed(0.15);
        set_moving_angle(0);
        is_key = true;
        set_spriteSheet(get_body_walk(&tick));
        set_delay(tick);
        light_add_angle += delta/250;
    }

    if (_events->IsKeyDown(_left)){
        set_speed(0.075);
        set_moving_angle(-90);
        is_key = true;
        set_spriteSheet(get_body_walk(&tick));
        set_delay(tick);
    }

    if (_events->IsKeyDown(_right)){
        set_speed(0.075);
        set_moving_angle(90);
        is_key = true;
        set_spriteSheet(get_body_walk(&tick));
        set_delay(tick);
    }

    if (_events->IsKeyDown(_run)){
        set_speed(0.25);
        set_moving_angle(0);
        is_key = true;
        set_spriteSheet(get_body_run(&tick));
        light_add_angle += delta/100;
        set_delay(tick);
    }

    if (!is_key){
        set_speed(0);
        set_spriteSheet(get_body_idle(&tick));
        set_delay(tick); 
        light_add_angle += delta/1000;
    }

    if (_events->isButtonDown(event::Mouse_button_left)){
        shoot();
    }
}


void P::set_run_key(SDL_Scancode key){
    _run = key;
}