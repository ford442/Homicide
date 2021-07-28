#pragma once

#include <iostream>
#include <SDL2/SDL_gpu.h>
#include "xml.hpp"

namespace light{
    class LightImage{
        public:
            LightImage(float *pixel_size, float *x, float *y);
            LightImage(XMLNode* node, float *pixel_size, float *x, float *y);
            LightImage(std::string path, std::string name, float *pixel_size, float *x, float *y);
            ~LightImage();

            bool load(std::string path, std::string name);
            bool load(XMLNode* node);
            bool load_csv(std::string path);

            GPU_Image* get_image(void) const;
            std::string get_name(void) const;

            void draw(GPU_Target* t, const int x, const int y, const float angle);
            
        private:

            GPU_Image* _image;
            std::string _name;

            SDL_FPoint center;

            bool load(std::string path);

            float *_x;
            float *_y;
            float *_pixel_size;
            float _scale;

    };
}