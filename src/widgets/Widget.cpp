#include "widgets/Widget.hpp"

Widget::Widget(){
    pos(0, 0);
    size(0, 0);
}

Widget::~Widget(){

}

void Widget::OnTick(const int delta){

}

void Widget::OnDraw(void){

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