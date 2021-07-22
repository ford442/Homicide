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

        private:
            std::list<std::shared_ptr<Weapon_type>> _weapons;

            bool push_weapon(std::shared_ptr<Weapon_type> w);
    };
}