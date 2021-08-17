#include "sprites/SpriteSheet.hpp"
#include <filesystem>
#include <SDL2/SDL_gpu.h>
#include <SDL2/SDL_image.h>
#include "dir.hpp"
#include "csv.hpp"

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
            std::cerr << "ERROR :: IMG_Load : " << IMG_GetError()<< ", cannot load \"" << file << "\"" << std::endl;
            continue;
        }

        _sprites[i] = GPU_CopyImageFromSurface(surface);
        SDL_FreeSurface(surface);
        
    }
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

int S::size(void) const{
    return _sprites_len;
}

void S::secure_id(int *id) const{
    if (*id > size()) *id = size();
}

GPU_Image* S::get_image(int id){
    return _sprites[id];
}

bool S::load_csv(std::string path){
    CSV::Document doc;

    if (!doc.load(path)) return false;

    _name = doc.search("name");

    std::string images_path, files_name, files_extension;

    images_path = doc.search("path");
    files_name = doc.search("file_name");
    files_extension = doc.search("extension");

    if (!load(images_path, files_name, files_extension)) return false;
    return true;
}