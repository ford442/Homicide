#include "world/World.hpp"
#include "xml.hpp"
#include "dir.hpp"
#include <cstring>
#include <SDL2/SDL_image.h>
#include "entity/Player.hpp"

using W = world::World;

W::World(std::shared_ptr<sprite::Animations> animations, float *pixel_size, float *x, float *y, std::shared_ptr<light::LightImageList> lightImageList, std::shared_ptr<entity::Player> player, std::shared_ptr<World> self, std::shared_ptr<event::Handler> events, std::shared_ptr<entity::EntityList> entity_list) : _animations(animations), _pixel_size(pixel_size), _x(x), _y(y), _lightImageList(lightImageList), _player(player), _self(self), _entityList(entity_list){
    std::cout << "INFO :: allocating World instance" << std::endl;
    init_ptr();
}

W::~World(){
    std::cout << "INFO :: releasing memory from a World instance" << std::endl;
    _floor = nullptr;
    _top = nullptr;
    _light = nullptr;
    _lightImageList = nullptr;
    _player = nullptr;
    _collisions = nullptr;
    _Astar = nullptr;

    images.clear();
    sprites.clear();

    if (enlightened_target) GPU_FreeTarget(enlightened_target);
    if (enlightened_image) GPU_FreeImage(enlightened_image);
}

void W::draw(GPU_Target *t){

    GPU_Clear(enlightened_target);
    _light->draw(t);
    _floor->draw(enlightened_target);

    for (auto &i : images){
        i->blit(enlightened_target);
    }

    for (auto &s : sprites){
        s->OnDraw(enlightened_target);
    }

    _entityList->draw(enlightened_target);
}

void W::blit(GPU_Target *t){
    enlightened_image->base_w = enlightened_image->w * *_pixel_size;
    enlightened_image->base_h = enlightened_image->h * *_pixel_size;
    GPU_Rect src = {0, 0, float(enlightened_image->base_w), float(enlightened_image->base_h)};
    GPU_Blit(enlightened_image, &src, t, -*_x + (enlightened_image->w * *_pixel_size) / 2, -*_y + (enlightened_image->h * *_pixel_size) / 2);
}

inline char* getPathAttribute(XMLNode* node){
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (!strcmp(attr.key, "path")){
            return attr.value;
        }
    }
    return nullptr;
}

bool W::load(std::string file, std::shared_ptr<event::Handler> events){
    if (file[1] != ':') file = DATA + file;

    #ifdef WORLD_LOGS
        std::cout << "INFO :: world : load world datas from \"" << file << "\"" << std::endl;
    #endif

    XMLDocument doc;
    bool error = false;

    world_w = -1;
    world_h = -1;

    if (XMLDocument_load(&doc, file.c_str())){
        #ifdef WORLD_LOGS
            std::cout << "INFO :: world : file reading success, querry datas" << std::endl;
        #endif

        for (int n=0; n<doc.root->children.size; n++){
            XMLNode* node = XMLNode_child(doc.root, n);

            #ifdef WORLD_LOGS
                std::cout << "\tINFO :: world : node \"" << node->tag << "\"" << std::endl;
            #endif

            if (!strcmp(node->tag, "floor")){
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: world : floor node found" << std::endl;
                #endif
                char* path = getPathAttribute(node);

                if (path){
                    #ifdef WORLD_LOGS
                        std::cout << "\tINFO :: world : floor source image path found : \"" << path << "\"" << std::endl;
                    #endif
                    _floor = std::make_unique<Floor>(_pixel_size);
                    if (!_floor->load(path)){
                        XMLDocument_free(&doc);
                        return false;
                    }
                } else {
                    #ifdef WORLD_ERRS
                        std::cerr << "ERROR :: world : cannot found the \"path\" attribute of floor in \"" << file << "\"" << std::endl;
                    #endif
                    error = true;
                    break;
                }
            } else  if (!strcmp(node->tag, "top")){
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: world : top node found" << std::endl;
                #endif
                char* path = getPathAttribute(node);

                if (path){
                    #ifdef WORLD_LOGS
                        std::cout << "\tINFO :: top source image path found : \"" << path << "\"" << std::endl;
                    #endif
                    _top = std::make_unique<Top>(_pixel_size, _x, _y);
                    if (!_top->load(path)){
                        XMLDocument_free(&doc);
                        return false;
                    }
                } else {
                    #ifdef WORLD_ERRS
                        std::cerr << "ERROR :: cannot found the \"path\" attribute of floor in \"" << file << "\"" << std::endl;
                    #endif
                    error = true;
                    break;
                }
            } else  if (!strcmp(node->tag, "light-collisions")){
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: world : ilght-collisions node found" << std::endl;
                #endif
                char* path = getPathAttribute(node);

                if (path){
                    #ifdef WORLD_ERRS
                        std::cout << "\tINFO :: light collisions source image path found : \"" << path << "\"" << std::endl;
                    #endif
                    _light = std::make_shared<LightSurface>(_pixel_size, _x, _y, &ambient_color, _lightImageList);
                    if (!_light->load(path)){
                        XMLDocument_free(&doc);
                        return false;
                    }
                } else {
                    #ifdef WORLD_ERRS
                        std::cerr << "ERROR :: cannot found the \"path\" attribute of floor in \"" << file << "\"" << std::endl;
                    #endif
                    error = true;
                    break;
                }
            } else  if (!strcmp(node->tag, "collisions")){
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: world : collisions node found" << std::endl;
                #endif

                _collisions = std::make_shared<Collisions>(_x, _y, _pixel_size);
                if (!_collisions->load(node)){
                    XMLDocument_free(&doc);
                    return false;
                }
            } else  if (!strcmp(node->tag, "size")){
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: world : size node found" << std::endl;
                    std::cout << "\tINFO :: world : read size attributes" << std::endl;
                #endif
                for (int a=0; a<node->attributes.size; a++){
                    XMLAttribute attr = node->attributes.data[a];

                    if (!strcmp(attr.key, "w")){
                        sscanf(attr.value, "%d", &world_w);
                    } else if (!strcmp(attr.key, "h")){
                        sscanf(attr.value, "%d", &world_h);
                    } else {
                        #ifdef WORLD_ERRS
                            std::cerr << "WARNING :: cannot reconize \"" << attr.key << "\" size attribute in \"" << file << "\"" << std::endl;
                        #endif
                    }
                }

                update_sizes();

                if (world_w == -1){
                    #ifdef WORLD_ERRS
                        std::cerr << "ERROR :: the width of the world is not set" << std::endl;
                    #endif
                    error = true;
                    break;
                }

                if (world_h== -1){
                    #ifdef WORLD_ERRS
                        std::cerr << "ERROR :: the height of the world is not set" << std::endl;
                    #endif
                    error = true;
                    break;
                }
            } else if (!strcmp(node->tag, "animations")){
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: world : animations node found" << std::endl;
                #endif
                if (!_animations->load(node)){
                    XMLDocument_free(&doc);
                    return false;
                }
            } else if (!strcmp(node->tag, "ambient-color")){
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: world : ambient-color node found" << std::endl;
                #endif
                for (int a=0; a<node->attributes.size; a++){
                    XMLAttribute attr = node->attributes.data[a];
                    if (!strcmp(attr.key, "r")){
                        int r;
                        sscanf(attr.value, "%d", &r);
                        ambient_color.r = r;

                    } else if (!strcmp(attr.key, "g")){
                        int g;
                        sscanf(attr.value, "%d", &g);
                        ambient_color.g = g;

                    } else if (!strcmp(attr.key, "b")){
                        int b;
                        sscanf(attr.value, "%d", &b);
                        ambient_color.b = b;
                        
                    } else if (!strcmp(attr.key, "a")){
                        int a;
                        sscanf(attr.value, "%d", &a);
                        ambient_color.a = a;

                    } else {
                        #ifdef WORLD_ERRS
                            std::cerr << "WARNING :: cannot reconize \"" << attr.key << "\" ambient light attribute in \"" << file << "\"" << std::endl;
                        #endif
                    }
                }
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: new ambient-color r:" << (int)ambient_color.r << " g:" << (int)ambient_color.g << " b:" << (int)ambient_color.b << " a:" << (int)ambient_color.a << std::endl;
                #endif
            } else if (!strcmp(node->tag, "light-images")){
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: world : light-images node found" << std::endl;
                #endif
                if (!_lightImageList->load(node)){
                    XMLDocument_free(&doc);
                    return false;
                }

            } else if (!strcmp(node->tag, "light")){
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: world : light node found" << std::endl;
                #endif
                if (_light == nullptr || !_light->load_world_light_image(node)) return false;
            
            } else if (!strcmp(node->tag, "player")){
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: world : player node found" << std::endl;
                #endif
                _player = std::make_shared<entity::Player>(_animations, _light, events, _x, _y, _pixel_size, _collisions);
                _player->load(node);
                _player->set_primary(weapons->get("mp5"));

            } else if (!strcmp(node->tag, "Astar")){
                #ifdef WORLD_LOGS
                    std::cout << "\tINFO :: world : Astar node found" << std::endl;
                #endif
                SDL_Surface* source = nullptr;
                int node_padding = 25;

                for (int a=0; a<node->attributes.size; a++){
                    XMLAttribute attr = node->attributes.data[a];

                    if (!strcmp(attr.key, "path")){
                        std::string value = attr.value;
                        if (value[1] != ':') value = RES + value;

                        source = IMG_Load(value.c_str());
                        if (!source){
                            #ifdef WORLD_ERRS
                                std::cerr << "ERROR :: IMG_Load : " << IMG_GetError() << "\" cannot load : \"" << value << "\" image file" << std::endl;
                            #endif
                            XMLDocument_free(&doc);
                            return false;
                        }
                    } else if (!strcmp(attr.key, "nodes-padding")){
                        sscanf(attr.value, "%d", &node_padding);

                    } else {
                        #ifdef WORLD_ERRS
                            std::cerr << "WRNING :: cannot reconize \"" << attr.key << "\" Astar attribute" << std::endl;
                        #endif
                    }
                }

                if (!source){
                    #ifdef WORLD_ERRS
                        std::cerr << "ERROR :: cannot load the Astar path finder, source image missing (use \"path=<source image path>\"" << std::endl;
                    #endif
                    XMLDocument_free(&doc);
                    return false;
                }

                _Astar = std::make_shared<A_star>(_pixel_size, _x, _y, _collisions);

                if (!_Astar->load(source, node_padding)){
                    XMLDocument_free(&doc);
                    return false;
                }

                _Astar->set_world_pix_size(world_w, world_h);
            } else if (!strcmp(node->tag, "enemy")){
                #ifdef WORLD_LOGS
                    std::cout << "INFO :: world : \"enemy\" node found" << std::endl;
                #endif

                std::shared_ptr<entity::Enemy> enemy = std::make_shared<entity::Enemy>(_animations, _light, _x, _y, _pixel_size, _collisions, _Astar);
                if (enemy->load(node)){
                    _entityList->push_back(enemy);
                }
            } else {
                #ifdef WORLD_ERRS
                    std::cerr << "WARNING :: cannot reconize \"" << node->tag << "\" in \"" << file << "\"" << std::endl;
                #endif
            }
        }
    } else {
        #ifdef WORLD_OUT_LOGS
            std::cout << "INFO :: world : loading, failure" << std::endl;
        #endif
        return false;
    }

    XMLDocument_free(&doc);

    #ifdef WORLD_OUT_LOGS
        std::cout << "INFO :: world loaded, success : " << (error == true ? "false" : "true") << std::endl;
    #endif

    _light->set_reset_color(ambient_color);
    return !error;
}

bool W::is_error(void){
    return _error;
}

void W::update_sizes(void){
    #ifdef WORLD_LOGS
        std::cout << "INFO :: world : update light image and target size : " << world_w << "x" << world_h << " : " << world_w * world_h << std::endl;
    #endif

    if (enlightened_image) GPU_FreeImage(enlightened_image);
    enlightened_image = GPU_CreateImage(world_w, world_h, GPU_FORMAT_RGBA);

    if (!enlightened_image){
        #ifdef WORLD_ERRS
            std::cerr << "ERROR :: GPU_CreateImage" << std::endl;
        #endif
        return;
    }

    GPU_SetImageFilter(enlightened_image, GPU_FILTER_NEAREST);
    GPU_SetBlendMode(enlightened_image, GPU_BLEND_MULTIPLY);

    if (enlightened_image) GPU_FreeTarget(enlightened_target);
    enlightened_target = GPU_LoadTarget(enlightened_image);

    if (!enlightened_target){
        #ifdef WORLD_ERRS
            std::cerr << "ERROR :: GPU_LoadTarget" << std::endl;
        #endif
        return;
    }

    #ifdef WORLD_LOGS
        std::cout << "INFO :: world : size updating success" << std::endl;
    #endif
}

std::shared_ptr<world::LightSurface> W::get_light(void){
    return _light;
}


void W::push_image(std::shared_ptr<light::IlluminatedImage> image){
    images.push_back(image);
}

std::shared_ptr<light::IlluminatedImage> W::push_image(void){
    std::shared_ptr<light::IlluminatedImage> image = std::make_shared<light::IlluminatedImage>();
    push_image(image);
    return image;
}

std::vector<std::shared_ptr<light::IlluminatedImage>>* W::get_images(void){
    return &images;
}

std::shared_ptr<entity::Player> W::get_player(void){
    return _player;
}

void W::push_sprite(std::shared_ptr<sprite::Sprite> sprite){
    sprites.push_back(sprite);
}

std::shared_ptr<sprite::Sprite> W::push_sprite(std::string type){
    std::shared_ptr<sprite::Sprite> sprite = std::make_shared<sprite::Sprite>(_animations);
    if (sprite->load_spriteSheet(type))
        push_sprite(sprite);
    return sprite;
}

std::vector<std::shared_ptr<sprite::Sprite>>* W::get_sprited(void){
    return &sprites;
}

void W::draw_top(GPU_Target *t, bool debug_mod){
    _top->draw(t);
    if (debug_mod) _light->draw_dev(t);
    _Astar->drawMap(t);
}

void W::draw_light_poly(GPU_Target *t){
    _light->draw_poly(t);
}

std::shared_ptr<world::Collisions> W::get_collisions(void) const{
    return _collisions;
}

void W::init_ptr(void){
    enlightened_image = nullptr;
    enlightened_target = nullptr;
    _floor = nullptr;
    _top = nullptr;
    _light = nullptr;
    _collisions = nullptr;
    _Astar = nullptr;
}

std::shared_ptr<world::A_star> W::get_Astar(void) const{
    return _Astar;
}

void W::OnTick(const float delta){
    // _Astar->OnTick(delta);
}

GPU_Target *W::get_enlightened_target(void) const{
    return enlightened_target;
}

void W::set_weapons(std::shared_ptr<weapons::Weapon_list> list){
    weapons = list;
}