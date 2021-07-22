#include "entity/EntityList.hpp"

using EL = entity::EntityList;
using E = entity::Entity;

EL::EntityList(){
    std::cout << "INFO :: allocating a EntityList instance" << std::endl;
    back = nullptr;
    front = nullptr;
    _size = 0;
}

EL::~EntityList(){
    std::cout << "INFO :: releasing memory from a EntityList instance" << std::endl;
    clear();
    back = nullptr;
    front = nullptr;
}

int EL::push_back(std::shared_ptr<E> e){
    std::shared_ptr<EntityElement> element = std::make_shared<EntityElement>();

    element->next = back;
    element->id = _size;
    element->value = e;
    back = element;
    _size++;

    return element->id;
}

void EL::pop_back(void){
    std::shared_ptr<EntityElement> temp = back;
    back = back->next;

    temp->next = nullptr;
    temp->value = nullptr;
    temp->id = 0;
    temp = nullptr;
    _size--;
}

void EL::pop(int id){
    std::shared_ptr<EntityElement> temp;
    temp = back;

    while (temp != nullptr && temp->id != id){
        temp = temp->next;
    }

    pop(temp);
}

void EL::pop(std::shared_ptr<Entity> e){
    std::shared_ptr<EntityElement> temp;
    temp = back;

    while (temp != nullptr && temp->value != e){
        temp = temp->next;
    }

    pop(temp);
}

void EL::draw(GPU_Target* t){
    std::shared_ptr<EntityElement> temp;
    temp = back;

    while (temp != nullptr){
        temp->value->OnDraw(t);
        temp = temp->next;
    }
}

void EL::updateMovements(const float delta){
    std::shared_ptr<EntityElement> temp;
    temp = back;

    while (temp != nullptr){
        temp->value->OnMouvementUpdate(delta);
        temp = temp->next;
    }
}

void EL::updateAnimations(const float delta){
    std::shared_ptr<EntityElement> temp;
    temp = back;

    while (temp != nullptr){
        temp->value->OnAnimationUpdate(delta);
        temp = temp->next;
    }
}

void EL::OnTick(const float delta){
    std::shared_ptr<EntityElement> temp;
    temp = back;

    while (temp != nullptr){
        temp->value->OnTick(delta);
        temp = temp->next;
    }
}

void EL::OnMouseClick(int mouse_btn){
    std::shared_ptr<EntityElement> temp;
    temp = back;

    while (temp != nullptr){
        temp->value->OnMouseClick(mouse_btn);
        temp = temp->next;
    }
}

void EL::OnMouseMovement(const int x, const int y){
    std::shared_ptr<EntityElement> temp;
    temp = back;

    while (temp != nullptr){
        temp->value->OnMouseMovement(x, y);
        temp = temp->next;
    }
}

void EL::clear(void){
    std::cout << "INFO :: clear a EntityList instance's elements" << std::endl;
    while (_size){
        pop_back();
    }
}

int EL::size(void) const{
    return _size;
}

void EL::update_size(void){
    std::shared_ptr<EntityElement> temp;
    temp = back;
    _size = 0;

    while (temp != nullptr){
        temp = temp->next;
        _size ++;
    }
}

void EL::pop(std::shared_ptr<EntityElement> e){
    std::shared_ptr<EntityElement> temp;
    temp = back;

    while (temp && temp->next != e){
        temp = temp->next;
    }

    temp->next = e->next;
    e->next = nullptr;
    e->value = nullptr;
    e->id = 0;
}


void EL::OnLightDraw(GPU_Target *t){
    std::shared_ptr<EntityElement> temp;
    temp = back;

    while (temp != nullptr){
        temp->value->OnLightDraw(t);
        temp = temp->next;
    }
}
