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
    LOG("TextButton loaded with success");
    return true;
}

bool TextButton::is_button(void) const{
    return true;
}

std::string TextButton::get(void) const{
    return get_path();
}