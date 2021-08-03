#pragma once

#include <iostream>
#include <string>
#include <list>
#include <memory>

#include "weapons/Weapon_type.hpp"

namespace weapons{
    class Weapon_list{
        public:
            Weapon_list();
            ~Weapon_list();

            bool load(std::string directory);

            bool is_weapon_exist(std::string name);
            std::shared_ptr<Weapon_type> get(std::string weapon);

        private:
            std::list<std::shared_ptr<Weapon_type>> _weapons;

            void push_weapon(std::shared_ptr<Weapon_type> w);
            bool load_weapon(std::string path);
    };
}