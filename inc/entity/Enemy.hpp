#pragma once

#include <iostream>
#include <memory>

#include "entity/Entity.hpp"
#include "entity/NPC.hpp"
#include "xml.hpp"

namespace entity{
    class Enemy : public NPC{
        public:
            Enemy(std::shared_ptr<sprite::Animations> animations, float *x, float *y, float *pixel_size, std::shared_ptr<world::Collisions> collisions, std::shared_ptr<world::A_star> A_star);
            ~Enemy();

            void OnDraw(GPU_Target *t) override;
            void OnTick(const float delta) override;
            void OnDestinationAchieved(void) override;

            bool load(XMLNode* node);
    };
}