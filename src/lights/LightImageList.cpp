#include "lights/LightImageList.hpp"
#include "dir.hpp"
#include <cstring>

using L = light::LightImageList;

L::LightImageList(float *pixel_size, float *x, float *y) : _pixel_size(pixel_size), _x(x), _y(y){
    std::cout << "INFO :: allocating a ListImageList instance" << std::endl;
}

L::~LightImageList(){
    std::cout << "INFO :: releasing memory from a LightImageList instance" << std::endl;
    _images.clear();
}

std::shared_ptr<light::LightImage> L::get(std::string name){
    for (auto &i :_images){
        if (i->get_name() == name) return i;
    }
    return nullptr;
}

void L::push(std::shared_ptr<LightImage> image){
    _images.push_back(image);
}

bool L::load(XMLNode *node){
    std::cout << "INFO :: load LightImage from a xml node" << std::endl;
    for (int c=0; c<node->children.size; c++){
        XMLNode* child = XMLNode_child(node, c);

        if (!strcmp(child->tag, "light-image")){
            std::shared_ptr<LightImage> l = std::make_shared<LightImage>(_pixel_size, _x, _y);
            if (l->load(child)) push(l);
        } else {
            std::cerr << "WARNING :: cannot reconize \"" << child->tag << "\" light-images child" << std::endl;
        }
    }
    return true;
}
