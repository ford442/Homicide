#ifndef __ENTITY_HPP__
    #define __ENTITY_HPP__

    #include <iostream>
    #include <memory>
    #include <SDL2/SDL_gpu.h>
    #include "xml.hpp"
    #include "world/Collisions.hpp"
    #include "sprites/Sprite.hpp"
    #include "sprites/SpriteSheet.hpp"
    #include "lights/ShadowBlock.hpp"

    class Entity : protected Sprite, public ShadowBlock{
        public:
            Entity();
            ~Entity();

            virtual void OnTick(const int delta);
            virtual void OnDraw(GPU_Target *t, const float x, const float y, const float zoom);
            virtual bool load(XMLNode *node, std::list<std::shared_ptr<sprite::SpriteSheet>> &sprites, ShadowCaster *shadowCaster);

            bool is_running(void);
            void run(void);
            void walk(void);

            const float x(void) const;
            const float y(void) const;
            void pos(float *x, float *y);

            void x(const float x);
            void y(const float y);
            void pos(const float x, const float y);

            const float facing(void) const;
            void facing(const float facing);

            const float direction(void) const;
            void direction(const float direction);

            const float run_speed(void) const;
            void run_speed(const float speed);

            const float walk_speed(void) const;
            void walk_speed(const float speed);

            void set_collisions(world::Collisions *collisions);

        private:
            float _x, _y;
            float _facing_angle;
            float _direction;

            float _run_speed;
            float _walk_speed;
            bool running;

            world::Collisions *collisions;

            bool load_xml_attribute(XMLNode *node);
        
        protected:

            const int str_to_int(std::string str);
            const float str_to_float(std::string str);
    };

#endif