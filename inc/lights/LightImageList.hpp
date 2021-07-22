#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "lights/LightImage.hpp"
#include "xml.hpp"

namespace light{
    class LightImageList{
        public:
            LightImageList(float *pixel_size, float *x, float *y);
            ~LightImageList();

            std::shared_ptr<LightImage> get(std::string name);
            void push(std::shared_ptr<LightImage> image);

            bool load(XMLNode *node);
        
        private:
            std::vector<std::shared_ptr<LightImage>> _images;

            float *_x;
            float *_y;
            float *_pixel_size;
    };
}