#include "sprites/SpriteSheet.hpp"
#include <filesystem>
#include <SDL2/SDL_gpu.h>
#include <SDL2/SDL_image.h>
#include "dir.hpp"

using S = sprite::SpriteSheet;

S::SpriteSheet(){
    std::cout << "INFO :: allocation a SpriteSheet instance" << std::endl;
    _sprites = nullptr;
    _sprites_len = 0;
}

S::SpriteSheet(std::string path, std::string files_name, std::string files_extension){
    std::cout << "INFO :: allocation a SpriteSheet instance" << std::endl;
    _sprites = nullptr;
    _sprites_len = 0;
    load(path, files_name, files_extension);
}

S::~SpriteSheet(){
    std::cout << "INFO :: releasing memory from a SpriteSheet instance" << std::endl;
    for (int i=0; i<_sprites_len; i++){
        if (_sprites[i]) GPU_FreeImage(_sprites[i]);
    }
}

bool S::load(std::string path, std::string files_name, std::string files_extension){
    if (path[1] != ':') path = RES + path;
    std::cout << "INFO :: load a spriteSheet from \"" << path << files_name << "<id>" << files_extension << "\"" << std::endl;
    while (true){
        std::string file = path + files_name + std::to_string(_sprites_len+1) + files_extension;
        std::cout << "INFO :: looking for \"" << file << "\" file" << std::endl;
        if (std::filesystem::exists(file)){
            _sprites_len ++;
            std::cout << "INFO :: file found" << std::endl;
        } else {
            std::cout << "INFO :: file not found" << std::endl;
            break;
        }
    }

    std::cout << "INFO :: files found : " << _sprites_len << std::endl;
    if (_sprites_len == 0) return false; 
    _sprites = std::make_unique<GPU_Image*[]>(_sprites_len);

    for (int i=0; i<_sprites_len; i++){
        _sprites[i] = nullptr;
        std::string file = path + files_name + std::to_string(i+1) + files_extension;
        std::cout << "INFO :: load \"" << file << "\"" << std::endl;

        SDL_Surface* surface = IMG_Load(file.c_str());

        if (!surface){
            std::cerr << "ERROR :: IMG_Load : " << IMG_GetError << ", cannot load \"" << file << "\"" << std::endl;
            continue;
        }

        _sprites[i] = GPU_CopyImageFromSurface(surface);
        SDL_FreeSurface(surface);
        
    }
    return true;
}

bool S::draw(GPU_Target* t, GPU_Rect* src_rect, const SDL_FPoint pos, const int id){
    if (!is_valid_id(id)) return false;
    GPU_Rect rect;
    if (!src_rect){
        rect = {0, 0, float(_sprites[id]->w), float(_sprites[id]->h)};
    } else {
        rect = *src_rect;
    }
    GPU_Blit(_sprites[id], &rect, t, pos.x, pos.y);
    return true;
}

bool S::drawScale(GPU_Target* t, GPU_Rect* src_rect, const SDL_FPoint pos, const SDL_FPoint scale, const int id){
    if (!is_valid_id(id)) return false;
    GPU_Rect rect;
    if (!src_rect){
        rect = {0, 0, float(_sprites[id]->w), float(_sprites[id]->h)};
    } else {
        rect = *src_rect;
    }
    GPU_BlitScale(_sprites[id], &rect,t, pos.x, pos.y, scale.x, scale.y);
    return true;
}

bool S::drawRotate(GPU_Target* t, GPU_Rect* src_rect, const SDL_FPoint pos, const float angle, const int id){
    if (!is_valid_id(id)) return false;
    GPU_Rect rect;
    if (!src_rect){
        rect = {0, 0, float(_sprites[id]->w), float(_sprites[id]->h)};
    } else {
        rect = *src_rect;
    }
    GPU_BlitRotate(_sprites[id], &rect, t, pos.x, pos.y, angle);
    return true;
}

bool S::drawTransforme(GPU_Target* t, GPU_Rect* src_rect, const SDL_FPoint pos, const float angle, const SDL_FPoint scale, const int id){
    if (!is_valid_id(id)) return false;
    GPU_Rect rect;
    if (!src_rect){
        rect = {0, 0, float(_sprites[id]->w), float(_sprites[id]->h)};
    } else {
        rect = *src_rect;
    }
    GPU_BlitTransform(_sprites[id], &rect, t, pos.x, pos.y, angle, scale.x, scale.y);
    return true;
}

bool S::is_valid_id(const int id){
    return id < _sprites_len && _sprites[id] != nullptr;
}

std::string S::name(void) const{
    return _name;
}

void S::name(std::string name){
    _name = name;
}

int S::id(void) const{
    return _id;
}

void S::id(const int id){
    _id = id;
}

int S::get_sprite_len(void) const{
    return _sprites_len;
}

void S::secure_id(int *id) const{
    if (*id > get_sprite_len()) *id = get_sprite_len();
}

GPU_Image* S::get_image(int id){
    return _sprites[id];
}