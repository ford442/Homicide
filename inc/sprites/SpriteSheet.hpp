#pragma once

#include <iostream>
#include <memory>
#include <SDL2/SDL_gpu.h>

namespace sprite{
    class SpriteSheet{
        public:
            SpriteSheet();
            SpriteSheet(std::string path, std::string files_name, std::string files_extension);
            ~SpriteSheet();

            bool load(std::string path, std::string files_name, std::string files_extension);

            bool draw(GPU_Target* t, GPU_Rect* src_rect, const SDL_FPoint pos, const int id);
            bool drawScale(GPU_Target* t, GPU_Rect* src_rect, const SDL_FPoint pos, const SDL_FPoint scale, const int id);
            bool drawRotate(GPU_Target* t, GPU_Rect* src_rect, const SDL_FPoint pos, const float angle, const int id);
            bool drawTransforme(GPU_Target* t, GPU_Rect* src_rect, const SDL_FPoint pos, const float angle, const SDL_FPoint scale, const int id);

            std::string name(void) const;
            void name(std::string name);

            int id(void) const;
            void id(const int id);

            int get_sprite_len(void) const;
            void secure_id(int *id) const;

            GPU_Image* get_image(int id);

        private:

            std::unique_ptr<GPU_Image*[]> _sprites;
            std::string _name;
            int _sprites_len;
            int _id;

            bool is_valid_id(const int id);
    };
}