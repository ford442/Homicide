#include "lights/ShadowBlock.hpp"
#include "main.hpp"
#include <cmath>

#define edge_angle std::get<0>
#define edge_dist std::get<1>
#define edge_ptr std::get<2>
#define edge_side std::get<3>


ShadowBlock::ShadowBlock(){
    pos(0, 0);
    angle(0);
    _size = 0;
}

ShadowBlock::~ShadowBlock(){

}

static inline bool str_to_bool(std::string str){
    if (str == "true")
        return true;
    return false;
}

bool ShadowBlock::load_edge(std::tuple<float, float, ShadowCaster::Edge*, std::array<bool, 4>> &edge, XMLNode *node){
    float x=INFINITY, y=INFINITY;

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "x")){
            try{
                x = std::stof(attr.value);
            } catch (std::exception &e){
                ERR("standart exception : " + std::string(e.what()));
            }
        } else if (is_equal(attr.key, "y")){
            try{
                y = std::stof(attr.value);
            } catch (std::exception &e){
                ERR("standart exception : " + std::string(e.what()));
            }
        
        } else if (is_equal(attr.key, "left")){
            edge_side(edge)[0] = str_to_bool(attr.value);
        
        } else if (is_equal(attr.key, "up")){
            edge_side(edge)[1] = str_to_bool(attr.value);
        
        } else if (is_equal(attr.key, "right")){
            edge_side(edge)[2] = str_to_bool(attr.value);
        
        } else if (is_equal(attr.key, "down")){
            edge_side(edge)[3] = str_to_bool(attr.value);

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" " + std::string(node->tag) + " attribute");
        }
    }

    if (x == INFINITY){
        ERR("cannot found the \"x\" attribute in " + std::string(node->tag));
        return false;
    }

    if (y == INFINITY){
        ERR("cannot found the \"y\" attribute in " + std::string(node->tag));
        return false;
    }

    edge_dist(edge) = sqrtf(pow(_x - x, 2) + pow(_y - y, 2));
    edge_angle(edge) = atan2(_y - y, _x - x);

    return true;
}

bool ShadowBlock::load(XMLNode *node, ShadowCaster *shadowCaster){
    try{
        edges = std::make_unique<std::tuple<float, float, ShadowCaster::Edge*, std::array<bool, 4>>[]>(node->children.size);
        _size = node->children.size;
    } catch (std::exception &e){
        ERR("standart exception : " + std::string(e.what()));
        return false;
    }

    for (int c=0; c<node->children.size; c++){
        XMLNode *child = XMLNode_child(node, c);
        if (!load_edge(edges[c], child)) return false;
    }

    auto edges_vec = shadowCaster->get_edges();

    for (int i=0; i<size(); i++){
        ShadowCaster::Edge edge;
        edges_vec.push_back(edge);
        edge_ptr(edges[i]) = &edges_vec.back();
    }

    return true;
}

const float ShadowBlock::x(void) const{
    return _x;
}

const float ShadowBlock::y(void) const{
    return _y;
}

const float ShadowBlock::angle(void) const{
    return _angle;
}

void ShadowBlock::pos(float *x, float *y) const{
    *x = this->x();
    *y = this->y();
}

void ShadowBlock::x(const float x){
    _x = x;
}

void ShadowBlock::y(const float y){
    _y = y;
}

void ShadowBlock::angle(const float angle){
    _angle = angle;
}

void ShadowBlock::pos(const float x, const float y){
    this->x(x);
    this->y(y);
}

const int ShadowBlock::size(void) const{
    return _size;
}