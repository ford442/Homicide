#pragma once

#include <iostream>
#include <memory>
#include <list>
#include <SDL2/SDL_gpu.h>
#include "xml.hpp"
#include "sprites/SpriteSheet.hpp"

class Sprite{
    public:
        Sprite();
        ~Sprite();

        void set_delay(const int delay);
        int get_delay(void) const;

        void set_id(const int id);
        int get_id(void) const;

        void OnTick(const int delta);
        void OnDraw(GPU_Target* t, const float x, const float y, const float zoom);
        bool load(XMLNode *node, std::list<std::shared_ptr<sprite::SpriteSheet>> &sprites);

        void pos(const float x, const float y);
        void x(const float x);
        void y(const float y);

        const float x(void) const;
        const float y(void) const;
        void pos(float *x, float *y) const;

        void set_angle(const float angle);

        bool is_paused(void) const;
        void pause(void);
        void play(void);

        void set_spriteSheet(std::shared_ptr<sprite::SpriteSheet> spriteSheet);
        std::shared_ptr<sprite::SpriteSheet> get_spriteSheet(void) const;
    
    private:
        std::shared_ptr<sprite::SpriteSheet> spriteSheet;
        std::string sprite_sheet_name;

        int _ticks;
        int _delay;
        
        int _id;

        float _x, _y;
        float _angle;
        float _scale;
        bool _pause;
};