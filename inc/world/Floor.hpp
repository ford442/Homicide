#pragma once

#include <iostream>
#include <memory>
#include <SDL2/SDL_gpu.h>
#include <SDL2/SDL.h>

namespace world{
    class Floor{
        public:
            Floor(float *pixel_size);
            ~Floor();

            bool load(std::string path);
            void draw(GPU_Target* t);

        private:
            float *_pixel_size;

            GPU_Image* _image;

            
    };
}