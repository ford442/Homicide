#pragma once

#include <iostream>
#include <list>

#include "entity/Entity.hpp"
#include "world/A-star.hpp"

namespace entity{
    class NPC : public Entity{
        public:
            NPC(std::shared_ptr<sprite::Animations> animations, float *x, float *y, float *pixel_size, std::shared_ptr<world::Collisions> collisions, std::shared_ptr<world::A_star> A_star);
            ~NPC();

            void OnTick(const float delta) override;
            void OnDraw(GPU_Target* t) override;

            void go_to(const float x, const float y);

            void walk(void);
            void run(void);
            void stop(void);

            void set_walk_speed(const float speed);
            void set_run_speed(const float speed);

            
            virtual void OnTargetAchieved(void);
            virtual void OnDestinationAchieved(void);

        
        private:
            world::Astar_nodes_path _path;
            world::A_star::PNode *_target;
            std::shared_ptr<world::A_star> _A_star;

            // 0:stop, 1:walk, 2:run
            int movement_type;

            void update_target_node(void);
            void update_speed(void);

            float walk_speed;
            float run_speed;

    };
};