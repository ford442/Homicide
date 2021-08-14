#include "widgets/TextButton.hpp"
#include "main.hpp"

TextButton::TextButton() : Button(), Text(){}
TextButton::~TextButton(){}

void TextButton::OnDraw(GPU_Target *target){
    Text::OnDraw(target);
}

bool TextButton::load(XMLNode *node){
    if (!Button::load(node)) return false;
    if (!Text::load(node)) return false;
    return true;
}

void TextButton::x(const float x){
    Button::x(x);
    Text::x(x);
}

void TextButton::y(const float y){
    Button::y(y);
    Text::y(y);
}

const float TextButton::x(void) const{
    return Button::x();
}

const float TextButton::y(void) const{
    return Button::y();
}