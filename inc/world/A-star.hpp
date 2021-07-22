#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <tuple>
#include <vector>
#include <SDL2/SDL_gpu.h>

#include "world/Collisions.hpp"

namespace world{

    struct Astar_nodes_path;
    struct Path_node;

    class A_star{
        public:
            A_star(float *zoom, float *x, float *y, std::shared_ptr<Collisions>);
            ~A_star();

            void drawMap(GPU_Target *t);
            void set_world_size(const int w, const int h);
            void set_world_pix_size(const int w, const int h);

            struct PNode{
                bool is_obstacle {false};
                int x;
                int y;

                std::vector<PNode*> neighbours;

                PNode(world::A_star::PNode*& p){}
                PNode(){}
            };

            void calculate(const int sx, const int sy, const int ex, const int ey, PNode** start, PNode** end, Path_node* nodes);
            void calculate_vec(const int sx, const int sy, const int ex, const int ey, Astar_nodes_path *l, Path_node* nodes);

            bool load(SDL_Surface *source, const float padding);

            int get_w(void) const;
            int get_h(void) const;

            int get_padding(void) const;
        
        private:
            int mapWidth, mapHeight;
            int world_w, world_h;

            float *_zoom;
            float *_x;
            float *_y;

            float nodes_padding;

            std::shared_ptr<PNode> _nodeStart;
            std::shared_ptr<PNode> _nodeEnd;
            
            std::unique_ptr<PNode[]> _nodes;
            std::shared_ptr<Collisions> _collisions;

            bool is_collision(SDL_Surface* surface, int x, int y);
            bool is_collisions_rect(SDL_Surface* surface, int cx, int cy, int w, int h);
    };

    // functions used for multythreading
    struct Path_node{
        bool visited;
        float local_goal;
        float global_goal;
        A_star::PNode* parent;
        A_star::PNode* target;

        Path_node(world::Path_node*& p){}
        Path_node(){}
    };
    
    struct Astar_nodes_path{
        std::vector<A_star::PNode*> list;
        std::unique_ptr<Path_node[]> table;

        bool calculating{false};
    };

    struct Astar_update{
        float sx, sy;
        float ex, ey;

        Astar_nodes_path *nodes;
        Path_node* visited_table;
        A_star* astar;
    };

    int calculate_Astar_ptr(void *ptr);
    bool calculate_Astar(float start_x, float start_y, float end_x, float end_y, Astar_nodes_path *l, A_star* a);
}