#include "sprites/Animations.hpp"
#include "dir.hpp"
#include "xml.hpp"
#include <cstring>

using A = sprite::Animations;

A::Animations(){
    std::cout << "INFO :: allocating a Animation instance" << std::endl;
}

A::~Animations(){
    std::cout << "INFO :: releasing memory from a Animation instance" << std::endl;
    _sprites.clear();
}

std::shared_ptr<sprite::SpriteSheet> A::get_sprite(std::string name){
    for (auto &s : _sprites){
        if (s->name() == name) return s;
    }
    std::cerr << "ERROR :: cannot found the animation sprite \"" << name << "\"" << std::endl;
    return nullptr;
}

std::shared_ptr<sprite::SpriteSheet> A::get_sprite(int id){
    for (auto &s : _sprites){
        if (s->id() == id) return s;
    }
    return nullptr;
}

int A::get_sprite_id(std::string name){
    std::shared_ptr<sprite::SpriteSheet> sprite = get_sprite(name);
    if (sprite == nullptr) return 0;
    return sprite->id();
}

bool A::load(std::string file_path){
    if (file_path[1] != ':') file_path = RES + file_path;
    std::cout << "INFO :: load animations from \"" << file_path << "\"" << std::endl;

    XMLDocument doc;

    if (XMLDocument_load(&doc, file_path.c_str())){
        for (int n=0; n<doc.root->children.size; n++){
            XMLNode* node = XMLNode_child(doc.root, n);

            if (!strcmp(node->tag, "sprite")){
                std::shared_ptr<sprite::SpriteSheet> sprite = std::make_shared<sprite::SpriteSheet>();
                std::string name, path, file_name, extension;

                for (int a=0; a<node->attributes.size; a++){
                    XMLAttribute attr = node->attributes.data[a];

                    if (!strcmp(attr.key, "name")){
                        name = attr.value;
                    } else if (!strcmp(attr.key, "path")){
                        path = attr.value;
                    } else if (!strcmp(attr.key, "file_name")){
                        file_name = attr.value;
                    } else if (!strcmp(attr.key, "extension")){
                        extension = attr.value;
                    } else {
                        std::cerr << "WARNING :: cannot reconize \"" << attr.key << "\" sprite attribute in \"" << file_path << "\"" << std::endl;
                    }
                }

                sprite->name(name);
                sprite->id(_sprites.size());

                if (sprite->load(path, file_name, extension))
                    _sprites.push_back(sprite);

            } else {
                std::cerr << "WARNING :: cannot reconize \"" << node->tag << "\" node in \"" << file_path << "\"" << std::endl;
            }
        }
    } else {
        return false;
    }

    XMLDocument_free(&doc);
    return false;
}

bool A::load(XMLNode* node){
for (int n=0; n<node->children.size; n++){
        XMLNode* child = XMLNode_child(node, n);

        if (!strcmp(child->tag, "sprite")){
            std::shared_ptr<sprite::SpriteSheet> sprite = std::make_shared<sprite::SpriteSheet>();
            std::string name, path, file_name, extension;

            for (int a=0; a<child->attributes.size; a++){
                XMLAttribute attr = child->attributes.data[a];

                if (!strcmp(attr.key, "name")){
                    name = attr.value;
                } else if (!strcmp(attr.key, "path")){
                    path = attr.value;
                } else if (!strcmp(attr.key, "file_name")){
                    file_name = attr.value;
                } else if (!strcmp(attr.key, "extension")){
                    extension = attr.value;
                } else {
                    std::cerr << "WARNING :: cannot reconize \"" << attr.key << "\" sprite attribute" << std::endl;
                }
            }

            sprite->name(name);
            sprite->id(_sprites.size());

            if (sprite->load(path, file_name, extension))
                _sprites.push_back(sprite);

        } else {
            std::cerr << "WARNING :: cannot reconize \"" << child->tag << "\" node" << std::endl;
        }
    }
    return true;
}

void A::push(std::shared_ptr<sprite::SpriteSheet> sprite){
    _sprites.push_back(sprite);
}