#pragma once

#include <iostream>
#include <string>
#include <SDL2/SDL_gpu.h>
#include "xml.hpp"
#include "weapons/Projectile_type.hpp"

namespace weapons{
    class Weapon_type{
        public:
            Weapon_type();
            ~Weapon_type();

            bool load(std::string path);

            std::string get_name(void) const;
            void set_name(std::string name);

            bool is_valid_projecile(std::shared_ptr<Projectile_type> p);

            GPU_Image *get_image(void) const;
            GPU_Image *get_icon(void) const;

        private:

            std::string _name;

            void set_name_xml(XMLNode* node);
            void set_image_xml(XMLNode *node);
            bool set_image(std::string path);
            void load_position(XMLNode *node);
            void load_power(XMLNode *node);
            void load_node(XMLNode *node);
            void load_countdown(XMLNode *node);
            void load_cannon_pos(XMLNode *node);
            void load_precision(XMLNode *node);
            void load_mass(XMLNode *node);
            void load_recoil(XMLNode *node);
            void load_caliber(XMLNode *node);
            void load_icon_xml(XMLNode *node);
            bool load_icon(std::string path);

            void destroy_image(void);
            void destory_icon(void);

            GPU_Image *image;
            GPU_Image *icon;

            float x;
            float y;

            float cannon_x;
            float cannon_y;

            float power;
            int countdown;
            float precision;
            float mass;
            float recoil;
            float caliber;
    };
};