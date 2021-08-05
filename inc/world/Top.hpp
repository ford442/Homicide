#pragma once

#include <iostream>
#include <memory>
#include <SDL2/SDL_gpu.h>
#include <SDL2/SDL.h>

namespace world{
    class Top{
        public:
            Top();
            ~Top();

            bool load(std::string path);
            GPU_Image *image(void) const;

        private:
            GPU_Image* _image;

            
    };
}