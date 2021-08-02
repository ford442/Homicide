#ifndef __PROJECTILE_HPP__
    #define __PROJECTILE_HPP__

    #include <iostream>
    #include <string>
    #include <memory>
    #include <SDL2/SDL_gpu.h>
    #include "weapons/Projectile_type.hpp"
    #include "entity/Entity.hpp"
    #include "lights/LightSource.hpp"
    #include "world/Collisions.hpp"

    class Projectile{
        public:
            Projectile();
            ~Projectile();

            void set_type(std::shared_ptr<Projectile_type> type);
            void draw(GPU_Target *t, float x, float y, float zoom);

            bool OnTick(const int delta, std::shared_ptr<world::Collisions> collisions, int* contact_normal);

            void set_angle(const float angle);
            void update_dir(void);

            void set_x(const float x);
            void set_y(const float y);

            void set_owner(std::shared_ptr<entity::Entity> owner);
            std::shared_ptr<entity::Entity> get_owner(void);
        
        private:
            std::shared_ptr<Projectile_type> _type;

            float x, y;
            float dir_x, dir_y;

            float angle;

            std::shared_ptr<entity::Entity> owner;
    };

#endif