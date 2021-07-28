#pragma once

#include <iostream>
#include <memory>
#include <SDL2/SDL_gpu.h>

#include "sprites/SpriteSheet.hpp"
#include "sprites/Animations.hpp"

namespace sprite{
    class Sprite{
        public:
            Sprite(std::shared_ptr<sprite::Animations> animations);
            ~Sprite();

            void set_delay(const int delay);
            int get_delay(void) const;
            int *get_delay_ptr(void);

            void set_id(const int id);
            int get_id(void) const;

            void OnTick(const int delta);
            void OnDraw(GPU_Target* t);

            void set_pos(const float x, const float y);
            void set_x(const float x);
            void set_y(const float y);

            void set_angle(const float angle);

            float *get_x_ptr(void);
            float *get_y_ptr(void);
            void get_pos_ptr(float *x, float *y);
            float *get_angle_ptr(void);

            void pause(void);
            void play(void);

            void set_spriteSheet(std::shared_ptr<sprite::SpriteSheet> spriteSheet);
            bool load_spriteSheet(std::string sprite_type);
            std::shared_ptr<sprite::SpriteSheet> get_spriteSheet(void) const;

        
        private:

            std::shared_ptr<sprite::SpriteSheet> _spriteSheet;

            int _ticks;
            int _delay;
            
            // add one to id when ticks is greater than delay and sub _delay to tick
            int _id;

            float _x, _y;
            float _angle;

            bool _pause;

            std::shared_ptr<sprite::Animations> _animations;
    };
}