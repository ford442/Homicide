#include "widgets/Rect.hpp"
#include <sstream>
#include "main.hpp"

Rect::Rect() : Widget(){
    set_color(0, 0, 0, 0);
}

Rect::~Rect(){

}

void Rect::OnDraw(GPU_Target *t){
    const float w = this->w() / 2;
    const float h = this->h() / 2;

    GPU_RectangleFilled(t, x()-w, y()-h, x()+w, y()+h, fg);
}

void Rect::set_color(SDL_Color color){
    fg = color;
}

static inline SDL_Color colorConverter(int hexValue){
    SDL_Color rgbColor;
    rgbColor.r = (hexValue >> 24) & 0xFF;
    rgbColor.g = (hexValue >> 16) & 0xFF;
    rgbColor.b = (hexValue >> 8)& 0xFF;
    rgbColor.a = hexValue & 0xff;

    return rgbColor;
}

static inline void convert_hexa_to_Uint32(std::string hex_src, Uint32 *dst){
    std::stringstream ss;
    ss << std::hex << hex_src;
    ss >> *dst;
}

static inline SDL_Color str_to_rgb(std::string str){
    Uint32 hex;
    convert_hexa_to_Uint32(str, &hex);
    return colorConverter(hex);
}

bool Rect::load(XMLNode *node){

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

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" square widget attribute");
        }
    }

    return true;
}

void Rect::set_color(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a){
    fg = {r, g, b, a};
}

SDL_Color Rect::get_color(void) const{
    return fg;
}

void Rect::set_color(std::string hexa){
    set_color(str_to_rgb(hexa));
}