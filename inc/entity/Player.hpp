#pragma once

#include <iostream>
#include <SDL2/SDL.h>

#include "entity/Entity.hpp"
#include "lights/LightSource.hpp"
#include "events/Event_handler.hpp"
#include "world/World.hpp"
#include "world/Collisions.hpp"
#include "xml.hpp"

namespace world{class World;}

namespace entity{
    class Player : public Entity{
        public:
            Player(std::shared_ptr<sprite::Animations> animations, std::shared_ptr<world::LightSurface> lightSurface, std::shared_ptr<event::Handler> events, float *x, float *y, float *pixel_size, std::shared_ptr<world::Collisions> collisions);
            ~Player();

            void OnMouseMovement(const int x, const int y) override;
            void OnMouseClick(int mouse_btn) override;
            void OnTick(const float delta) override;

            void load(XMLNode* node);
            
            void set_forward_key(SDL_Scancode key);
            void set_backward_key(SDL_Scancode key);
            void set_left_key(SDL_Scancode key);
            void set_right_key(SDL_Scancode key);
            void set_run_key(SDL_Scancode key);
        
        private:
            std::shared_ptr<world::World> _world;
            std::shared_ptr<light::LightSource> _light;
            std::shared_ptr<event::Handler> _events;

            float light_add_angle;

            SDL_Scancode _forward;
            SDL_Scancode _backward;
            SDL_Scancode _left;
            SDL_Scancode _right;
            SDL_Scancode _run;
    };
}
