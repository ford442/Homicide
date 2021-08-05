#pragma once

#include <iostream>
#include <memory>
#include <SDL2/SDL_gpu.h>
#include <SDL2/SDL.h>

namespace world{
    class Floor{
        public:
            Floor();
            ~Floor();

            bool load(std::string path);
            GPU_Image *image(void) const;

        private:
            float *_pixel_size;

            GPU_Image* _image;

            
    };
}