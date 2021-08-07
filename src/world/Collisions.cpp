#include "world/Collisions.hpp"
#include <sstream>
#include <cstring>
#include <algorithm>
#include <SDL2/SDL_image.h>
#include "dir.hpp"
#include "main.hpp"

using C = world::Collisions;

static inline Uint32 get_pixel32(SDL_Surface *surface, int x, int y){
    Uint32 *pixels = (Uint32*)surface->pixels;
    return pixels[(y * surface->w) + x];
}

C::Collisions(){
    CONSTRUCT();
    _world = nullptr;
}

C::~Collisions(){
    DESTRUCT();
    _world = nullptr;
}

void convert_hexa_to_Uint32(std::string hex_src, Uint32 *dst){
    std::stringstream ss;
    ss << std::hex << hex_src;
    ss >> *dst;
    std::cout << hex_src << " : 0x" << std::hex << *dst << std::endl;
}

bool load_color_node(XMLNode *node, Uint32 *color){
    bool node_found = false;
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "color")){
            std::string value = attr.value;
            std::size_t pos = value.find("0x");

            if (pos != std::string::npos)
                value = value.erase(pos);
            
            #if SDL_BYTEORDER != SDL_BIG_ENDIAN
                std::reverse(value.begin(), value.end());
            #endif

            convert_hexa_to_Uint32(value, color);
            node_found = true;
        } else {
            std::cerr << "WARNING :: cannot reconize \"" << attr.key << "\" attribute of a color node" << std::endl;
        }
    }

    if (!node_found){
        std::cerr << "WARNING :: cannot found the \"" << color << "\" attribute of a color node" << std::endl;
        return false;
    }
    return true;
}

bool C::load(XMLNode *node){
    LOG("load from a xml node");
    for (int c=0; c<node->children.size; c++){
        XMLNode *child = XMLNode_child(node, c);

        if (is_equal(child->tag, "none")){
            load_color_node(child, &colors.none);

        } else if (is_equal(child->tag, "enemys_shoots")){
            load_color_node(child, &colors.enemys_shoots);

        } else if (is_equal(child->tag, "enemys")){
            load_color_node(child, &colors.none);
            
        } else if (is_equal(child->tag, "enemys_and_enemys_shoots")){
            load_color_node(child, &colors.enemys_and_enemys_shoots);
            
        } else if (is_equal(child->tag, "player_shoots")){
            load_color_node(child, &colors.player_shoots);
            
        } else if (is_equal(child->tag, "all_shoots")){
            load_color_node(child, &colors.all_shoots);
            
        } else if (is_equal(child->tag, "player_shoots_and_enemys")){
            load_color_node(child, &colors.player_shoots_and_enemys);
            
        } else if (is_equal(child->tag, "player_and_enemys_shoots_plus_enemys")){
            load_color_node(child, &colors.player_and_enemys_shoots_plus_enemys);
            
        } else if (is_equal(child->tag, "player")){
            load_color_node(child, &colors.player);
            
        } else if (is_equal(child->tag, "player_and_enemys_shoots")){
            load_color_node(child, &colors.player_and_enemys_shoots);
            
        } else if (is_equal(child->tag, "player_and_enemys")){
            load_color_node(child, &colors.player_and_enemys);
            
        } else if (is_equal(child->tag, "player_and_enemys_plus_enemys_shoots")){
            load_color_node(child, &colors.player_and_enemys_plus_enemys_shoots);
            
        } else if (is_equal(child->tag, "player_and_player_shoots")){
            load_color_node(child, &colors.player_and_player_shoots);
            
        } else if (is_equal(child->tag, "player_and_all_shoots")){
            load_color_node(child, &colors.player_and_all_shoots);
            
        } else if (is_equal(child->tag, "player_and_player_shoots_plus_enemys")){
            load_color_node(child, &colors.player_and_player_shoots_plus_enemys);
            
        } else if (is_equal(child->tag, "all")){
            load_color_node(child, &colors.all);
            
        } else {
            WARN("cannot reconize \"" + std::string(child->tag) + "\" collision xml node child");
        }
    }

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "path")){
            if (!load(attr.value)) return false;
        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" collision xml node attribute");
        }
    }

    std::cout << std::dec;
    return true;
}

bool C::load(std::string image_path){
    if (image_path[1] != ':') image_path = RES + image_path;
    LOAD_LOG(image_path);

    SDL_Surface* surface = IMG_Load(image_path.c_str());

    if (!surface){
        ERR("IMG_Load : " + std::string(IMG_GetError()));
        return false;
    }

    world_w = surface->w;
    world_h = surface->h;

    ALLOC_LOG("Collision table : " + std::to_string(surface->w * surface->h));
    _world = std::make_unique<Collision_type[]>(surface->w * surface->h);

    for (int y=0; y<surface->h; y++){
        for (int x=0; x<surface->w; x++){
            _world[y * surface->w + x] = get_type_from_color(get_pixel32(surface, x, y));
        }
    }

    SDL_FreeSurface(surface);
    return true;
}

C::Collision_type C::get_type_from_color(Uint32 color){
    if (color == colors.none){
        return Collision_none;

    } else if (color == colors.enemys_shoots){
        return Collision_enemys_shoots;

    } else if (color == colors.enemys){
        return Collision_enemys;

    } else if (color == colors.enemys_and_enemys_shoots){
        return Collision_enemys_and_enemys_shoots;

    } else if (color == colors.player_shoots){
        return Collision_player_shoots;

    } else if (color == colors.all_shoots){
        return Collision_all_shoots;

    } else if (color == colors.player_shoots_and_enemys){
        return Collision_player_shoots_and_enemys;

    } else if (color == colors.player_and_enemys_shoots_plus_enemys){
        return Collision_player_and_enemys_shoots_plus_enemys;

    } else if (color == colors.player){
        return Collision_player;

    } else if (color == colors.player_and_enemys_shoots){
        return Collision_player_and_enemys_choots;

    } else if (color == colors.player_and_enemys){
        return Collision_player_and_enemys;

    } else if (color == colors.player_and_enemys_plus_enemys_shoots){
        return Collision_player_and_enemys_plus_enemys_shoots;

    } else if (color == colors.player_and_player_shoots){
        return Collision_player_and_player_shoots;

    } else if (color == colors.player_and_all_shoots){
        return Collision_player_and_all_shoots;

    } else if (color == colors.player_and_player_shoots_plus_enemys){
        return Collision_player_and_player_shoots_plus_enemys;

    } else if (color == colors.all){
        return Collision_all;
    }

    std::cerr << "WARNING :: cannot reconize the given collision color \"" << std::hex << color << "\"" << std::endl;
    return Collision_none;
}

Uint32 C::get_color_from_type(C::Collision_type type){
    if (type == Collision_none){
        return colors.none;

    } else if (type == Collision_enemys_shoots){
        return colors.enemys_shoots;

    } else if (type == Collision_enemys){
        return colors.enemys;

    } else if (type == Collision_enemys_and_enemys_shoots){
        return colors.enemys_and_enemys_shoots;

    } else if (type == Collision_player_shoots){
        return colors.player_shoots;

    } else if (type == Collision_all_shoots){
        return colors.all_shoots;

    } else if (type == Collision_player_shoots_and_enemys){
        return colors.player_shoots_and_enemys;

    } else if (type == Collision_player_and_enemys_shoots_plus_enemys){
        return colors.player_and_enemys_shoots_plus_enemys;

    } else if (type == Collision_player){
        return colors.player;

    } else if (type == Collision_player_and_enemys_choots){
        return colors.player_and_enemys_shoots;

    } else if (type == Collision_player_and_enemys){
        return colors.player_and_enemys;

    } else if (type == Collision_player_and_enemys_plus_enemys_shoots){
        return colors.player_and_enemys_plus_enemys_shoots;

    } else if (type == Collision_player_and_player_shoots){
        return colors.player_and_player_shoots;

    } else if (type == Collision_player_and_all_shoots){
        return colors.player_and_all_shoots;

    } else if (type == Collision_player_and_player_shoots_plus_enemys){
        return colors.player_and_player_shoots_plus_enemys;

    } else if (type == Collision_all){
        return colors.all;
    }

    std::cerr << "WARNING :: ths given collision color type, id : \"" << type << "\"" << std::endl;
    return colors.none;
}

C::Collision_type C::get(const float x, const float y){
    if (x < 0 || y < 0) return Collision_none;
    if (x > world_w || y > world_h) return Collision_none; 

    return _world[y * world_w + x];
}

bool C::is_player_collision(Collision_type type){
    switch (type){
        case Collision_player_and_enemys_shoots_plus_enemys:
            return true;
        
        case Collision_player:
            return true;
        
        case Collision_player_and_enemys_choots:
            return true;
        
        case Collision_player_and_enemys:
            return true;
        
        case Collision_player_and_enemys_plus_enemys_shoots:
            return true;
        
        case Collision_player_and_player_shoots:
            return true;
        
        case Collision_player_and_all_shoots:
            return true;
        
        case Collision_player_and_player_shoots_plus_enemys:
            return true;
        
        case Collision_all:
            return true;
        
        default:
            return false;
    }
}

bool C::is_player_shoot_collision(Collision_type type){
    switch (type){
        case Collision_all_shoots:
            return true;
        
        case Collision_player_shoots_and_enemys:
            return true;
        
        case Collision_player_and_player_shoots:
            return true;
        
        case Collision_player_and_all_shoots:
            return true;
        
        case Collision_player_and_player_shoots_plus_enemys:
            return true;
        
        case Collision_all:
            return true;
        
        case Collision_player_shoots:
            return true;
        
        default:
            return false;
    }
}

bool C::is_enemy_collision(Collision_type type){
    switch (type){
        case Collision_enemys:
            return true;
        
        case Collision_enemys_and_enemys_shoots:
            return true;
        
        case Collision_player_shoots_and_enemys:
            return true;
        
        case Collision_player_and_enemys_shoots_plus_enemys:
            return true;
        
        case Collision_player_and_enemys:
            return true;
        
        case Collision_player_and_enemys_plus_enemys_shoots:
            return true;
        
        case Collision_player_and_player_shoots_plus_enemys:
            return true;
        
        case Collision_all:
            return true;
        
        default:
            return false;
    }
}

bool C::is_enemy_shoot_collision(Collision_type type){
    switch (type){
        case Collision_enemys_shoots:
            return true;
        
        case Collision_enemys_and_enemys_shoots:
            return true;
        
        case Collision_all_shoots:
            return true;
        
        case Collision_player_and_enemys_shoots_plus_enemys:
            return true;
        
        case Collision_player_and_enemys_choots:
            return true;
        
        case Collision_player_and_enemys_plus_enemys_shoots:
            return true;
        
        case Collision_player_and_all_shoots:
            return true;
        
        case Collision_all:
            return true;

        default:
            return false;
    }
}

bool C::is_collision(int x, int y, Collision_type type){
    Collision_type pixel = get(x, y);

    switch (type){
        case Collision_det_player:
            return is_player_collision(pixel);
        
        case Collision_det_player_shoot:
            return is_player_shoot_collision(pixel);

        case Collision_det_enemy:
            return is_enemy_collision(pixel);

        case Collision_det_enemy_shoot:
            return is_enemy_shoot_collision(pixel);
        
        default:
            return type == pixel;
    }
}

bool C::is_shoot_collision(Collision_type type){
    switch (type){
        case Collision_enemys_shoots:
            return true;
        
        case Collision_enemys_and_enemys_shoots:
            return true;
        
        case Collision_player_shoots:
            return true;
        
        case Collision_all_shoots:
            return true;

        case Collision_player_shoots_and_enemys:
            return true;
        
        case Collision_player_and_enemys_shoots_plus_enemys:
            return true;
        
        case Collision_player_and_enemys_choots:
            return true;
        
        case Collision_player_and_enemys_plus_enemys_shoots:
            return true;
        
        case Collision_player_and_player_shoots:
            return true;
        
        case Collision_player_and_all_shoots:
            return true;
        
        case Collision_player_and_player_shoots_plus_enemys:
            return true;
        
        case Collision_all:
            return true;
        
        default:
            return false;
    }
}


float C::get_dist(const int sx, const int sy, const Direction dir, float distance, Collision_type type){
    int dist_max = 0;

    if (distance < 1.0f) return distance;
    if (distance < 0.0f) distance *= -1;

    switch (dir){
        case dir_north:
            for (int y=0; y>-distance; y--){
                if (is_collision(sx, y + sy, type)) break;
                dist_max ++;
            }
            break;
        
        case dir_west:
            for (int x=0; x>-distance; x--){
                if (is_collision(x + sx, sy, type)) break;
                dist_max ++;
            }
            break;
        
        case dir_south:
            for (int y=0; y<distance; y++){
                if (is_collision(sx, y + sy, type)) break;
                dist_max ++;
            }
            break;
        
        case dir_east:
            for (int x=0; x<distance; x++){
                if (is_collision(x + sx, sy, type)) break;
                dist_max ++;
            }
            break;
        
        default:
            break;
    }

    return dist_max;
}

SDL_FPoint C::cast_ray(float x1, float y1, float x2, float y2){
    float m;

    if (x2 != x1){
        m = (y2 - y1) / (x2 - x1);
    }

    int increment = 1;
    SDL_FPoint contact;

    if (x2 != x1 && std::abs(m) <= 1.0f){
        
        if (x1 > x2){
            increment = -1;
        }

        const float m = (y2 - y1) / (x2 - x1);
        const float b = y1 - m * x1;

        if (increment == 1){
            for (int x = (int)x1; x<(int)x2; x++){
                const float y = m * (float)x + b;
                if (is_shoot_collision(get(x, y))){
                    contact = {float(x), float(y)};
                    return contact;
                }
            }
        } else {
            for (int x = (int)x1; x>(int)x2; x--){
                const float y = m * (float)x + b;
                if (is_shoot_collision(get(x, y))){
                    contact = {float(x), float(y)};
                    return contact;
                }
            }
        }
    } else {
        if (y1 > y2){
            increment = -1;
        }
        const float w = (x2 - x1) / (y2 - y1);
        const float p = x1 - w * y1;

        if (increment == 1){
            for (int y = (int)y1; y<(int)y2; y++){
                const float x = w * (float)y + p;
                if (is_shoot_collision(get(x, y))){
                    contact = {float(x), float(y)};
                    return contact;
                }
            }
        } else {
            for (int y = (int)y1; y>(int)y2; y--){
                const float x = w * (float)y + p;
                if (is_shoot_collision(get(x, y))){
                    contact = {float(x), float(y)};
                    return contact;
                }
            }
        }
    }

    contact = {x2, y2};
    return contact;
}

float C::cast_ray(const float x, const float y, const float angle){
    return 0.0f;
}