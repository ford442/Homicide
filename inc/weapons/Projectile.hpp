#ifndef __PROJECTILE_HPP__
    #define __PROJECTILE_HPP__

    #include <iostream>
    #include <string>
    #include <memory>
    #include <SDL2/SDL_gpu.h>
    #include "weapons/Projectile_type.hpp"
    #include "entity/Entity.hpp"
    #include "lights/LightSource.hpp"

    class Projectile{
        public:
            Projectile();
            ~Projectile();

            void set_type(std::shared_ptr<Projectile_type> type);
            void draw(GPU_Target *t);

            void OnTick(const int delta);

            void set_angle(const float angle);
            void update_dir(void);

            void set_x(const float x);
            void set_y(const float y);

            void set_owner(std::shared_ptr<entity::Entity> owner);
            std::shared_ptr<entity::Entity> get_owner(void);

            void set_light(std::shared_ptr<light::LightImage> light);
            std::shared_ptr<light::LightSource> get_light(void);
        
        private:
            std::shared_ptr<Projectile_type> _type;
            std::shared_ptr<light::LightSource> light;

            float x, y;
            float dir_x, dir_y;

            float angle;

            std::shared_ptr<entity::Entity> owner;
    };

#endif