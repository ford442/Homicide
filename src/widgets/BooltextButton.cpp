#include "widgets/BoolTextButton.hpp"
#include "main.hpp"

BoolTextButton::BoolTextButton() : TextButton(){
    value = nullptr;
}

BoolTextButton::~BoolTextButton(){

}

void BoolTextButton::set_value(bool *value){
    this->value = value;
}

void BoolTextButton::OnDraw(GPU_Target *t){
    TextButton::OnDraw(t);
    
    if (value){
        if (*value){
            GPU_CircleFilled(t, x() - w()/1.5f, y(), h()/4, {0, 255, 0, 255});
        } else {
            GPU_CircleFilled(t, x() - w()/1.5f, y(), h()/4, {255, 0, 0, 255});
        }
    } else {
        GPU_CircleFilled(t, x() - w()/1.5f, y(), h()/4, {150, 150, 150, 150});
    }
}

bool BoolTextButton::load(XMLNode *node){
    if (!TextButton::load(node)) return false;

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "value_name")){
            value_name = attr.value;
        }
    }

    if (value_name.empty()){
        ERR("cannot load BooltextButton without any value linked");
    }

    return true;
}

std::string BoolTextButton::get_value_name(void) const{
    return value_name;
}