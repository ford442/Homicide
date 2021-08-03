#include "weapons/Weapon_type_list.hpp"
#include "dir.hpp"
#include <cstring>

using namespace weapons;

#define is_equal(str1, str2) !strcmp(str1, str2)
#define ERR(msg) std::cerr << "ERROR :: in " << __FILE__ << " at : " << __func__ << " : " << msg << std::endl;
#define LOG(msg) std::cout << "INFO :: in " << __FILE__ << " at : " << __func__ << " : " << msg << std::endl;

Weapon_list::Weapon_list(){

}

Weapon_list::~Weapon_list(){
    _weapons.clear();
}

bool Weapon_list::load(std::string directory){
    if (directory[1] != ':') directory = RES + directory;
    LOG("loading weapons from \"" + directory + "\"");

    std::vector<std::string> files = dir::content(directory);
    if (files.empty()){
        ERR("the given directory dosent exist \"" + directory + "\"");
        return false;
    }

    for (auto &f : files){
        if (f == "."|| f == "..") continue;
        load_weapon(directory + f);
    }

    LOG("weapon loaded : " + std::to_string(_weapons.size()));
    return true;
}

bool Weapon_list::is_weapon_exist(std::string name){
    for (auto &w : _weapons){
        if (w->get_name() == name) return true;
    }
    return false;
}

void Weapon_list::push_weapon(std::shared_ptr<Weapon_type> w){
    LOG("push weapon type : \"" + w->get_name() + "\"");
    _weapons.push_back(w);
    LOG("weapon pushed")
}

bool Weapon_list::load_weapon(std::string path){
    std::shared_ptr<weapons::Weapon_type> type = std::make_shared<weapons::Weapon_type>();
    
    if (type->load(path)){
        push_weapon(type);
    } else {
        ERR("error occured during the loading of \"" + std::string(path) + "\" weapon file");
        return false;
    }
    return true;
}

std::shared_ptr<Weapon_type> Weapon_list::get(std::string weapon){
    for (auto &w : _weapons){
        if (w->get_name() == weapon) return w;
    }
    return nullptr;
}