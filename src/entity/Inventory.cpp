#include "entity/Inventory.hpp"

Inventory::Inventory(){
    primary = nullptr;
    secondary = nullptr;
}

Inventory::~Inventory(){

}

std::shared_ptr<weapons::Weapon_type> Inventory::get_primary(void) const{
    return primary;
}

std::shared_ptr<weapons::Weapon_type> Inventory::get_secondary(void) const{
    return secondary;
}

void Inventory::set_primary(std::shared_ptr<weapons::Weapon_type> weapon){
    primary = weapon;
}

void Inventory::set_secondary(std::shared_ptr<weapons::Weapon_type> weapon){
    secondary = weapon;
}