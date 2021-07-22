#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <xml.hpp>
#include "sprites/SpriteSheet.hpp"

namespace sprite{
    class Animations{
        public:
            Animations();
            ~Animations();

            std::shared_ptr<sprite::SpriteSheet> get_sprite(std::string name);
            std::shared_ptr<sprite::SpriteSheet> get_sprite(int id);
            int get_sprite_id(std::string name);

            bool load(std::string file_path);
            bool load(XMLNode* node);
            void unload(void);

        private:
            std::vector<std::shared_ptr<sprite::SpriteSheet>> _sprites;
            
    };
}