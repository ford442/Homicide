#pragma once

#include <iostream>
#include <memory>
#include <SDL2/SDL_gpu.h>
#include <string>
#include "world/Floor.hpp"
#include "world/LightSurface.hpp"
#include "world/Top.hpp"
#include "world/Collisions.hpp"
#include "world/A-star.hpp"
#include "lights/LightImageList.hpp"
#include "lights/IlluminatedImage.hpp"
#include "sprites/Animations.hpp"
#include "sprites/Sprite.hpp"
#include "entity/Player.hpp"
#include "entity/Enemy.hpp"
#include "entity/EntityList.hpp"
#include "weapons/Projectile_type.hpp"

namespace entity{
    class Entity;
    class Player;
    class EntityList;
    class Enemy;
}

namespace world{
    class World{
        public:
            World(std::shared_ptr<sprite::Animations> animations, float *pixel_size, float *x, float *y, std::shared_ptr<light::LightImageList> lightImageList, std::shared_ptr<entity::Player> player, std::shared_ptr<World> self, std::shared_ptr<event::Handler> events, std::shared_ptr<entity::EntityList> entity_list);
            ~World();

            void draw(GPU_Target *t);
            void blit(GPU_Target *t);
            void draw_light_poly(GPU_Target *t);
            void draw_top(GPU_Target *t, bool debug_mod);

            void OnTick(const float delta);

            bool load(std::string file, std::shared_ptr<event::Handler> events);
            bool is_error(void);

            std::shared_ptr<LightSurface> get_light(void);
            std::shared_ptr<Collisions> get_collisions(void) const;

            void push_image(std::shared_ptr<light::IlluminatedImage> image);
            std::shared_ptr<light::IlluminatedImage> push_image(void);
            std::vector<std::shared_ptr<light::IlluminatedImage>>* get_images(void);

            std::shared_ptr<entity::Player> get_player(void);

            void push_sprite(std::shared_ptr<sprite::Sprite> sprite);
            std::shared_ptr<sprite::Sprite> push_sprite(std::string type);
            std::vector<std::shared_ptr<sprite::Sprite>>* get_sprited(void);

            std::shared_ptr<A_star> get_Astar(void) const;
            GPU_Target *get_enlightened_target(void) const;

            void set_weapons(std::shared_ptr<weapons::Weapon_list> list);


        private:

            GPU_Image* enlightened_image;
            GPU_Target* enlightened_target;

            std::unique_ptr<Floor> _floor;
            std::unique_ptr<Top> _top;
            std::shared_ptr<LightSurface> _light;
            std::shared_ptr<light::LightImageList> _lightImageList;
            std::shared_ptr<Collisions> _collisions;
            std::shared_ptr<A_star> _Astar;
            

            std::shared_ptr<sprite::Animations> _animations;
            std::vector<std::shared_ptr<light::IlluminatedImage>> images;
            std::vector<std::shared_ptr<sprite::Sprite>> sprites;
            std::shared_ptr<entity::Player> _player;
            std::shared_ptr<entity::EntityList> _entityList;
            std::shared_ptr<World> _self;
            std::shared_ptr<weapons::Weapon_list> weapons;

            bool _error;
            int world_w, world_h;
            SDL_Color ambient_color;
            
            void update_sizes(void);

            void init_ptr(void);

            float *_pixel_size;
            float *_x;
            float *_y;
    };
}