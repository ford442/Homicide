#pragma once

#include <iostream>
#include <string>
#include <SDL2/SDL_gpu.h>
#include "xml.hpp"

namespace weapons{
    class Weapon_type{
        public:
            Weapon_type();
            ~Weapon_type();

            bool load(std::string path);

            std::string get_name(void) const;
            void set_name(std::string name);

        private:

            std::string _name;

            void set_name_xml(XMLNode* node);
            void set_image_xml(XMLNode *node);
            bool set_image(std::string path);

            void destroy_image(void);

            GPU_Image *image;

            float x;
            float y;

            float power;
    };
};