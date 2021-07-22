#include "weapons/Weapon_list.hpp"
#include "dir.hpp"

using W = weapons::Weapon_list;

#define CONSTRUCT_LOGS
#define DECONSTRUCT_LOGS
#define INIT_LOGS
#define INIT_WARNS
#define INIT_ERRS
#define _LOG_
#define _ERR_
#define _WARN_

#ifdef CONSTRUCT_LOGS
    #define CONS_LOGS(value) value;
#else
    #define CONS_LOGS(value) {}
#endif

#ifdef DECONSTRUCT_LOGS
    #define DECONS_LOGS(value) value;
#else
    #define DECONS_LOGS(value) {}
#endif 

#ifdef INIT_LOGS
    #define INIT_LOG(value) value;
#else
    #define INIT_LOG(value) {}
#endif 

#ifdef INIT_WARNS
    #define INIT_WARN(value) value;
#else
    #define INIT_WARN(value) {}
#endif 

#ifdef INIT_ERRS
    #define INIT_ERR(value) value;
#else
    #define INIT_ERR(value) {}
#endif 

#ifdef _LOG_
    #define LOG(value) value;
#else
    #define LOG(value) {}
#endif

#ifdef _ERR_
    #define ERR(value) value;
#else
    #define ERR(value) {}
#endif

#ifdef _WARN_
    #define WARN(value) value;
#else
    #define WARN(value) {}
#endif

W::Weapon_list(){
    CONS_LOGS(std::cout << "INFO :: Weapon_list : new Weapon_list instance" << std::endl);
}

W::~Weapon_list(){
    DECONS_LOGS(std::cout << "INFO :: Weapon_list : releasing memory from a Weapon_list instance" << std::endl);
}

bool W::load(std::string directory){
    if (directory[1] != ':') directory = RES + directory;
    LOG(std::cout << "INFO :: Weapon_list : load weapons from \"" << directory << "\" directory, eevry file as a Weapon_type xml file" << std::endl);

    std::vector<std::string> dir_content = dir::content(directory);
    LOG(std::cout << "INFO :: Weapon_list : directory opened, " << std::to_string(dir_content.size()) << " file(s) to read" << std::endl);

    if (dir_content.size() == 0){
        WARN(std::cerr << "WARNING :: Weapon_list : directory is empty" << std::endl);
    }

    for (std::string &s : dir_content){
        std::shared_ptr<Weapon_type> w = std::make_shared<Weapon_type>();

        if (w->Init(s)){
            push_weapon(w);
        }
    }
    return true;
}

bool W::push_weapon(std::shared_ptr<Weapon_type> w){
    if (!is_weapon_exist(w->get_name())){
        _weapons.push_back(w);
        LOG(std::cout << "INFO :: Weapon_list : new weapon : \"" << w->get_name() << "\"" << std::endl);
        return true;
    }

    WARN(std::cerr << "INFO :: Weapon_list : cannot push weapon \"" << w->get_name() << "\" : name early used" << std::endl);
    return false;
}

bool W::is_weapon_exist(std::string name){
    for (std::shared_ptr<Weapon_type> &w : _weapons){
        if (w->get_name() == name) return true;
    }
    return false;
}
