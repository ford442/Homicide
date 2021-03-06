#include "widgets/Widget.hpp"
#include "main.hpp"

Widget::Widget(){
    pos(0, 0);
    size(0, 0);
    set_window_size(nullptr, nullptr);
}

Widget::~Widget(){

}

void Widget::OnTick(const int delta){

}

void Widget::OnDraw(GPU_Target *t){

}

void Widget::OnHUDdraw(GPU_Target *t){

}

const float Widget::x(void) const{
    return _x;
}

const float Widget::y(void) const{
    return _y;
}

void Widget::pos(float *x, float *y) const{
    *x = this->x();
    *y = this->y();
}

void Widget::x(const float x){
    _x = x;
}

void Widget::y(const float y){
    _y = y;
}

void Widget::pos(const float x, const float y){
    this->x(x);
    this->y(y);
}

const float Widget::w(void) const{
    return _w;
}

const float Widget::h(void) const{
    return _h;
}

void Widget::size(float *w, float *h) const{
    *w = this->w();
    *h = this->h();
}

void Widget::w(const float w){
    _w = w;
}

void Widget::h(const float h){
    _h = h;
}

void Widget::size(const float w, const float h){
    this->w(w);
    this->h(h);
}

void Widget::set_events(event::Handler *handler){
    events = handler;
}

bool Widget::load(XMLNode *node){
    return true;
}

float Widget::str_to_float(std::string str){
    bool is_pourcent = false;
    bool pourcent_side = 0; // 0 = width, 1 = height

    if (str.size() > 2){
        if (str[1] == '%'){
            is_pourcent = true;

            if (str[0] == 'w' || str[0] == 'W'){
                pourcent_side = 0;
            } else if (str[0] == 'h' || str[0] == 'H'){
                pourcent_side = 1;
            } else {
                WARN("cannot reconize \"" + str[0] + std::string("\" side"));
            }
        }
        
        str.erase(1, 1);
        str.erase(0, 1);
    }
    
    float value = 0.0f;

    try {
        value = std::stof(str);
    } catch (std::exception &e){
        ERR("standart exception : " + std::string(e.what()));
        return 0.0f;
    }

    if (is_pourcent){
        value /= 100;

        if (pourcent_side == 0){
            if (window_w)
                return *window_w * value;
            return 0;
        } else {
            if (window_h)
                return *window_h * value;
            return 0;
        }
    } else {
        return value;
    }
}

float Widget::str_to_int(std::string str){
    bool is_pourcent = false;
    bool pourcent_side = 0; // 0 = width, 1 = height

    if (str.size() > 2){
        if (str[1] == '%'){
            is_pourcent = true;

            if (str[0] == 'w' || str[0] == 'W'){
                pourcent_side = 0;
            } else if (str[0] == 'h' || str[0] == 'H'){
                pourcent_side = 1;
            } else {
                WARN("cannot reconize \"" + str[0] + std::string("\" side"));
            }
        }
        
        str.erase(1, 1);
        str.erase(0, 1);
    }

    int value = 0.0f;

    try {
        value = std::stoi(str);
    } catch (std::exception &e){
        ERR("standart exception : " + std::string(e.what()));
        return 0.0f;
    }

    if (is_pourcent){
        value /= 100;

        if (pourcent_side == 0){
            if (window_w)
                return *window_w * value;
            return 0;
        } else {
            if (window_h)
                return *window_h * value;
            return 0;
        }
    } else {
        return value;
    }
}

void Widget::set_window_size(int *w, int *h){
    window_w = w;
    window_h = h;
}

bool Widget::is_mouse_hover(void){
    const float w = this->w() / 2;
    const float h = this->h() / 2;

    const int mx = events->mouse_x();
    const int my = events->mouse_y();

    if (mx >= x() - w && mx <= x() + w)
        if (my >= y() - h && my <= y() + h)
            return true;
    
    return false;
}

bool Widget::is_button(void) const{
    return false;
}

std::string Widget::get(void) const{
    return "";
}

bool Widget::is_switch(void) const{
    return false;
}

void Widget::active(void){
    
}