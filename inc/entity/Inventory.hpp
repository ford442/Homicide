#ifndef __INVENTORY__HPP__
    #define __INVENTORY__HPP__

    #include <iostream>
    #include <memory>
    #include <vector>

    #include "weapons/Weapon_type.hpp"
    #include "weapons/Weapon_type_list.hpp"
    #include "weapons/Projectile_type.hpp"

    class Inventory{
        public:
            Inventory();
            ~Inventory();

            std::shared_ptr<weapons::Weapon_type> get_primary(void) const;
            std::shared_ptr<weapons::Weapon_type> get_secondary(void) const;

            void set_primary(std::shared_ptr<weapons::Weapon_type> weapon);
            void set_secondary(std::shared_ptr<weapons::Weapon_type> weapon);

        private:
            std::shared_ptr<weapons::Weapon_type> primary;
            std::shared_ptr<weapons::Weapon_type> secondary;
    };
    
#endif
