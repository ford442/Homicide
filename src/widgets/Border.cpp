#include "widgets/Border.hpp"
#include "main.hpp"

Border::Border() : Rect(){
    border_width(10);
}

Border::~Border(){

}

void Border::OnDraw(GPU_Target *t){
    GPU_SetLineThickness(width);
    const float w = this->w() / 2;
    const float h = this->h() / 2;

    GPU_Rectangle(t, x()-w, y()-h, x()+w, y()+h, get_color());
    GPU_SetLineThickness(1);
}

bool Border::load(XMLNode *node){
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "x")){
            x(str_to_float(attr.value));

        } else if (is_equal(attr.key, "y")){
            y(str_to_float(attr.value));

        } else if (is_equal(attr.key, "w")){
            w(str_to_float(attr.value));

        } else if (is_equal(attr.key, "h")){
            h(str_to_float(attr.value));

        } else if (is_equal(attr.key, "fg")){
            set_color(attr.value);

        } else if (is_equal(attr.key, "border_width")){
            border_width(str_to_float(attr.value));

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" border widget attribute");
        }
    }

    return true;
}

const float Border::border_width(void) const{
    return width;
}

void Border::border_width(const float width){
    this->width = width;
}