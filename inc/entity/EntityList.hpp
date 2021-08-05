#pragma once

#include <iostream>
#include <memory>
#include <SDL2/SDL_gpu.h>
#include "entity/Entity.hpp"

namespace entity{
    class EntityList{
        public:
            EntityList();
            ~EntityList();

            int push_back(std::shared_ptr<Entity> e);
            void pop_back(void);
            void pop(int id);
            void pop(std::shared_ptr<Entity> e);
            void clear(void);

            void draw(GPU_Target* t);
            void drawHUD(GPU_Target *t);
            void updateMovements(const float delta);
            void updateAnimations(const float delta);
            void OnTick(const float delta);
            void OnLightDraw(GPU_Target *t);
            void OnMouseClick(int mouse_btn);
            void OnMouseMovement(const int x, const int y);

            int size(void) const;
            void update_size(void);

        private:

            struct EntityElement{
                int id;
                std::shared_ptr<Entity> value;
                std::shared_ptr<EntityElement> next;
            };

            std::shared_ptr<EntityElement> front;
            std::shared_ptr<EntityElement> back;

            int _size;

            void pop(std::shared_ptr<EntityElement> e);
    };
}