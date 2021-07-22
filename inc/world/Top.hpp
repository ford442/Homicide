#pragma once

#include <iostream>
#include <memory>
#include <SDL2/SDL_gpu.h>
#include <SDL2/SDL.h>

namespace world{
    class Top{
        public:
            Top(float *pixel_size, float *x, float *y);
            ~Top();

            bool load(std::string path);
            void draw(GPU_Target* t);

        private:
            float *_pixel_size;
            float *_x;
            float *_y;

            GPU_Image* _image;

            
    };
}