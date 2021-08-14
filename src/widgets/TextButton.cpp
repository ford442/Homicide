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