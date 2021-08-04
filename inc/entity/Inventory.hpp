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
            std::shared_ptr<weapons::Weapon_type> get_selected(void) const;
            const int get_selector(void) const;

            void set_primary(std::shared_ptr<weapons::Weapon_type> weapon);
            void set_secondary(std::shared_ptr<weapons::Weapon_type> weapon);
            void set_seletector(const int selector);

        private:
            int selector;

            std::shared_ptr<weapons::Weapon_type> primary;
            std::shared_ptr<weapons::Weapon_type> secondary;
    };
    
#endif
