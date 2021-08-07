#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <SDL2/SDL_gpu.h>
#include "sprites/Animations.hpp"
#include "xml.hpp"

#include "lights/LightSource.hpp"
#include "sprites/Sprite.hpp"
#include "world/Collisions.hpp"
#include "weapons/Weapon_type.hpp"
#include "entity/Inventory.hpp"

namespace world{class World;}

namespace entity{
    class Entity : public sprite::Sprite, public Inventory{
        public:
            Entity(std::shared_ptr<sprite::Animations> animations, float *x, float *y, float *pixel_size, std::shared_ptr<world::Collisions> collisions);
            ~Entity();
            
            // init
            bool load(XMLNode *node);

            // sprites
            std::shared_ptr<sprite::SpriteSheet> get_legs_walk(int *tick);
            std::shared_ptr<sprite::SpriteSheet> get_legs_run(int *tick);
            std::shared_ptr<sprite::SpriteSheet> get_legs_left(int *tick);
            std::shared_ptr<sprite::SpriteSheet> get_legs_right(int *tick);
            std::shared_ptr<sprite::SpriteSheet> get_body_idle(int *tick);
            std::shared_ptr<sprite::SpriteSheet> get_body_run(int *tick);
            std::shared_ptr<sprite::SpriteSheet> get_body_walk(int *tick);

            // location
            void set_x(const float x);
            void set_y(const float y);
            void set_location(const float x, const float y);

            float get_x(void) const;
            float get_y(void) const;
            void get_location(float *x, float *y) const;

            float *get_x_ptr(void);
            float *get_y_ptr(void);
            void get_location_ptr(float *x, float *y);

            // mouvements
            void set_speed(const float speed);
            float get_speed(void) const;
            float *get_speed_ptr(void);

            void set_facing_angle(const float angle);
            float get_facing_angle(void) const;
            float *get_facing_angle_ptr(void);

            void set_moving_angle(const float angle);
            float get_moving_angle(void) const;
            float *get_moving_angle_ptr(void);

            // events
            virtual void OnMouvementUpdate(const float delta);
            virtual void OnAnimationUpdate(const float delta);
            virtual void OnTick(const float delta);
            virtual void OnDraw(GPU_Target* t);
            virtual void HUD(GPU_Target *t);
            virtual void OnLightDraw(GPU_Target *t);
            virtual void OnMouseClick(int mouse_btn);
            virtual void OnMouseMovement(const int x, const int y);

            // types
            enum EntityAnimation{
                Entity_walk  = 0,
                Entity_run   = 1,
                Entity_left  = 2,
                Entity_right = 3,
                Entity_idle  = 4
            };
        
            void set_animation(const EntityAnimation animation);
            EntityAnimation get_animation(void) const;
            EntityAnimation *get_animation_ptr(void);

            void push_light(std::shared_ptr<light::LightSource> light);
            std::shared_ptr<light::LightSource> push_light(std::string light_type);
            std::vector<std::shared_ptr<light::LightSource>> *get_lights(void);

            void update_direction(void);
            SDL_FPoint get_direction(void);
            SDL_FPoint *get_direction_ptr(void);
            void set_direction(float x, float y);
            void set_direction(SDL_FPoint dir);

            float *_cam_x;
            float *_cam_y;
            float *_pixel_size;

            std::shared_ptr<world::Collisions> get_collisions(void) const;

            void shoot(void);

            float get_facing(void) const;

        private:

            float _x, _y;

            float _speed;
            float _facing_angle;
            float _moving_angle;

            SDL_FPoint _direction;
            
            std::shared_ptr<sprite::SpriteSheet> _legs_walk;
            int _legs_walk_ticks;
            
            std::shared_ptr<sprite::SpriteSheet> _legs_run;
            int _legs_run_ticks;
            
            std::shared_ptr<sprite::SpriteSheet> _legs_left;
            int _legs_left_ticks;

            std::shared_ptr<sprite::SpriteSheet> _legs_right;
            int _legs_right_ticks;
            
            std::shared_ptr<sprite::SpriteSheet> _legs_idle;
            int _legs_idle_ticks;

            std::shared_ptr<sprite::SpriteSheet> _body_idle;
            int _body_idle_ticks;

            std::shared_ptr<sprite::SpriteSheet> _body_run;
            int _body_run_ticks;

            std::shared_ptr<sprite::SpriteSheet> _body_walk;
            int _body_walk_ticks;
            
            std::shared_ptr<sprite::Animations> _animations;
            EntityAnimation _animation;

            void reset_animations(void);

            std::vector<std::shared_ptr<light::LightSource>> _lights;
            std::shared_ptr<sprite::Sprite> _legs;
            std::shared_ptr<world::Collisions> _collisions;
            int _collisions_radius;

            SDL_FPoint shoot_contact;
            bool shooted;
    };
}
