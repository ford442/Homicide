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

void Inventory::set_selector(const int selector){
    this->selector = selector;
}

std::shared_ptr<weapons::Weapon_type> Inventory::get_selected(void) const{
    switch (selector){
        case 1:
            return get_primary();
        
        case 2:
            return get_secondary();
        
        default:
            return nullptr;
    }
}

const int Inventory::get_selector(void) const{
    return selector;
}
