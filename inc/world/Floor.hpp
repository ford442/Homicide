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

            const float scale(void) const;
            void scale(const float value);
            void scale(std::string str);

        private:
            float *_pixel_size;

            GPU_Image* _image;
            float _scale;

            
    };
}