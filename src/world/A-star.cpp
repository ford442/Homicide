#include "world/A-star.hpp"
#include <cmath>
#include <filesystem>
#include <sstream>
#include <string>
#include <list>
#include <SDL2/SDL_thread.h>


// #define DEBUG_DISPLAY_ASTAR

using A = world::A_star;

Uint32 get_pixel32(SDL_Surface *surface, int x, int y);

bool A::is_collision(SDL_Surface* surface, int x, int y){
    return _collisions->is_enemy_collision(_collisions->get_type_from_color(get_pixel32(surface, x, y)));
}

bool A::is_collisions_rect(SDL_Surface* surface, int cx, int cy, int w, int h){
    const int sx = cx - w/2, ex = cx + w/2;
    const int sy = cy - h/2, ey = cy + h/2;

    for (int x=sx; x<ex; x++){
        for (int y=sy; y<ey; y++){
            if (x >= 0 && x <= surface->w && y >= 0 &&  y <= surface->h) if (is_collision(surface, x, y)) return true;
        }
    }

    return false;
}

A::A_star(float *zoom, float *x, float *y, std::shared_ptr<Collisions> collisions) : _zoom(zoom), _x(x), _y(y), _collisions(collisions){
    #ifdef ASTAR_LOGS
        std::cout << "INFO :: allocating a new A_star path finder instance" << std::endl;
    #endif
    _nodeEnd = nullptr;
    _nodeStart = nullptr;
}

A::~A_star(){
    #ifdef ASTAR_LOGS
        std::cout << "INFO :: releasing memory from a A_star instance" << std::endl;
    #endif
    _nodes = nullptr;
}

void A::drawMap(GPU_Target *t){
    
    #ifdef DEBUG_DISPLAY_ASTAR
        for (int y=0; y<mapHeight; y++){
            for (int x=0; x<mapWidth; x++){

                PNode* p = &_nodes[y  * mapWidth + x];

                int x1 = x * *_zoom * nodes_padding - *_x;
                int y1 = y * *_zoom * nodes_padding - *_y;

                SDL_Color col = {0, 0, 150, 255};

                if (p->is_obstacle){
                    col.r = 255;
                    col.b = 0;
                }

                for (auto &c : p->neighbours){
                    GPU_Line(t, x1, y1, c->x * *_zoom * nodes_padding - *_x, c->y * *_zoom * nodes_padding - *_y, {150, 150, 150, 255});
                }
                
                GPU_RectangleFilled(t, x1-3, y1-3, x1+3, y1+3, col);
            }
        }
    #endif
}

std::shared_ptr<A::PNode> getNodePos(std::vector<std::shared_ptr<A::PNode>> vec, const int x, const int y){
    for (auto &p : vec){
        if (p->x == x && p->y == y) return p;
    }
    return nullptr;
}

float dist(A::PNode* s, A::PNode* e){
    return sqrt(pow(s->x - e->x, 2) + pow(s->y - e->y, 2));
}

void A::calculate(const int sx, const int sy, const int ex, const int ey, PNode** start, PNode** end, Path_node* nodes){
    #ifdef ASTAR_LOGS
        std::cout << "INFO :: calculating A* : from " << sx << ";" << sy << " to " << ex << ";" << ey << std::endl;
    #endif

    int start_x = std::floor(sx / nodes_padding);
    int start_y = std::floor(sy / nodes_padding);
    int end_x = std::floor(ex / nodes_padding);
    int end_y = std::floor(ex / nodes_padding);

    PNode* start_node = &_nodes[start_y * mapWidth + start_x];
    *start = start_node;
    PNode* end_node = &_nodes[end_y * mapWidth + end_x];
    *end = end_node;

    Path_node* node_s = &nodes[start_y * mapWidth + start_x];
    Path_node* node_e = &nodes[end_y * mapWidth + end_x];

    if (end_node->is_obstacle) return;

    for (int y=0; y<mapHeight; y++){
        for (int x=0; x<mapWidth; x++){
            Path_node *n = &nodes[y * mapWidth + x];

            n->visited = false;
            n->global_goal = INFINITY;
            n->local_goal = INFINITY;
            n->parent = nullptr;
            n->target = &_nodes[y * mapWidth + x];
        }
    }

    #ifdef ASTAR_LOGS
        std::cout << "INFO :: Astar : rested node table values" << std::endl;
    #endif

    Path_node *currNode = &nodes[(*start)->y * mapWidth + (*start)->x];
    PNode* current_node = start_node;

    currNode->local_goal = 0.0f;
    currNode->global_goal = dist(start_node, end_node);

    std::list<Path_node*> notTested;
    notTested.push_back(node_s);

    #ifdef ASTAR_LOGS
        std::cout << "INFO :: Astar : path calulation" << std::endl;
    #endif

    while (!notTested.empty() && current_node != end_node){

        notTested.sort([](const Path_node* lhs, const Path_node* rhs){return lhs->global_goal < rhs->global_goal;});

        while (!notTested.empty() && nodes[notTested.front()->target->y * mapWidth + notTested.front()->target->x].visited == true){
            notTested.pop_front();
        }
        
        if (notTested.empty()){
            break;
        }

        currNode = notTested.front();
        current_node = currNode->target;
        nodes[current_node->y * mapWidth + current_node->x].visited = true;

        for (PNode* n : current_node->neighbours){
            Path_node *neighbhour = &nodes[n->y * mapWidth + n->x];

            if (neighbhour->visited == false && n->is_obstacle == false){
                notTested.push_back(neighbhour);
            }

            float possiblyLowerGlobal = currNode->local_goal + dist(current_node, neighbhour->target);

            if (possiblyLowerGlobal < neighbhour->local_goal){
                neighbhour->parent = current_node;
                neighbhour->local_goal = possiblyLowerGlobal;
                neighbhour->global_goal = neighbhour->local_goal + dist(neighbhour->target, end_node);
            }
        }
    }

    #ifdef ASTAR_LOGS
        std::cout << "INFO :: Astar : astar path calculation : success" << std::endl;
    #endif
}

void A::calculate_vec(const int sx, const int sy, const int ex, const int ey, Astar_nodes_path* l, Path_node* nodes){

    if (sx > world_w || sy > world_h || ex > world_w || ey > world_h || sx < 0 || sy < 0 || ex < 0 || ey < 0) return;

    l->calculating = true;
    l->list.clear();
    PNode *start{nullptr}, *end{nullptr};
    calculate(sx, sy, ex, ey, &start, &end, nodes);

    if (end == nullptr) return;

    #if defined(ASTAR_LOGS)
        std::cout << "INFO :: Astar : pushing nodes into list" << std::endl;
    #endif

    // count
    Path_node* node = &nodes[end->y * mapWidth + end->x];
    while (node->parent != nullptr){
        
        l->list.push_back(node->target);
        node = &nodes[node->parent->y * mapWidth + node->parent->x];
    }
    
    #if defined(ASTAR_LOGS)
        std::cout << "INFO :: Astar : nodes pushed" << std::endl;
    #endif
    l->calculating = false;
}

void A::set_world_size(const int w, const int h){
    mapWidth = w;
    mapHeight = h;
}

bool A::load(SDL_Surface *source, const float padding){
    nodes_padding = padding;
    #ifdef ASTAR_LOGS
        std::cout << "INFO :: Astar : load map table" << std::endl;
    #endif
    if (!source){
        #ifdef ASTAR_ERRS
            std::cerr << "ERROR :: Astar : cannot init the tap from a null source surface" << std::endl;
        #endif
        #ifdef ASTAR_OUT_LOGS
            std::cout << "INFO :: Astar : load : result (false)" << std::endl;
        #endif
        return false;
    }

    int w = source->w / nodes_padding;
    int h = source->w / nodes_padding;

    const float max = w * h;
    set_world_size(w, h);

    _nodes = std::make_unique<PNode[]>(max);
    #ifdef ASTAR_LOGS
        std::cout << "INFO :: A* node table size : " << w << "x" << h << " : " << max << std::endl;
    #endif
    
    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++){
            #ifdef ASTAR_LOGS_TABLE
                std::cout << "\tINFO : Astar : x : " << x << " y : " << y << std::endl;
            #endif

            PNode node;
            
            node.x = x;
            node.y = y;
            node.is_obstacle = is_collisions_rect(source, x * nodes_padding, y * nodes_padding, nodes_padding, nodes_padding);

            _nodes[y * w + x] = node;
            
        }
    }

    #ifdef ASTAR_LOGS
        std::cout << "INFO :: Astar : table loading : success" << std::endl;
    #endif

    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++){

            PNode* p = &_nodes[y  * w + x];
            
            if (y>0){
                p->neighbours.push_back(&_nodes[(y - 1) *w+ x]);
            }

            if (y<h-1){
                p->neighbours.push_back(&_nodes[(y + 1) * w + x]);
            }
            
            if (x>0){
                p->neighbours.push_back(&_nodes[y * w + (x - 1)]);
            }

            if (x<w-1){
                p->neighbours.push_back(&_nodes[y * w + (x + 1)]);
            }

            if (y>0 && x>0){
                p->neighbours.push_back(&_nodes[(y - 1) * w + (x - 1)]);
            }

            if (y<h-1 && x<w-1){
                p->neighbours.push_back(&_nodes[(y + 1) * w + (x + 1)]);
            }

            if (y>0 && x<w-1){
                p->neighbours.push_back(&_nodes[(y - 1) * w + (x + 1)]);
            }

            if (y<h-1 && x>0){
                p->neighbours.push_back(&_nodes[(y + 1) * w + (x - 1)]);
            }
        }
    }

    #ifdef ASTAR_OUT_LOGS
        std::cout << "INFO :: Astar : table loading and node vector loading : sucess; result (true)" << std::endl;
    #endif
    return true;
}

int A::get_w(void) const{
    return mapWidth;
}

int A::get_h(void) const{
    return mapHeight;
}



int world::calculate_Astar_ptr(void *ptr){
    if (!ptr){
        std::cerr << "ERROR :: Astar : cannot load the path from a null ptr" << std::endl;
        return false;
    }

    world::Astar_update* a = (world::Astar_update*)ptr;

    if (!a->nodes){
        std::cerr << "ERROR :: Astar : cannot push nodes into a null vector" << std::endl;
        return false;
    }

    if (!a->astar){
        std::cerr << "ERROR :: Astar : cannot find the path in a null A_star instance" << std::endl;
        return false;
    }

    std::cout << "INFO :: Astar : calculate multithreaded path from : " << a->sx << "x" << a->sy << " to " << a->ex << "x" << a->ey << std::endl;

    a->astar->calculate_vec(a->sx, a->sy, a->ex, a->ey, a->nodes, a->visited_table);
    delete a;
    std::cout << "INFO :: Astar : path calculation, success" << std::endl;
    return true;
}

bool world::calculate_Astar(float start_x, float start_y, float end_x, float end_y, world::Astar_nodes_path *l, A_star* a){
    if (l->calculating) return true;
    world::Astar_update *au = new world::Astar_update;

    au->sx = start_x;
    au->sy = start_y;
    au->ex = end_x;
    au->ey = end_y;
    au->nodes = l;
    au->astar = a;
    au->visited_table = l->table.get();

    std::string name = "Astar" + std::to_string(rand());

    std::cout << "INFO :: Astar : calculate astar path multithreaded" << std::endl;

    SDL_Thread *t = SDL_CreateThread(calculate_Astar_ptr, "Astar", au);
    return true;
}

int A::get_padding(void) const{
    return nodes_padding;
}

void A::set_world_pix_size(const int w, const int h){
    world_w = w;
    world_h = h;
}
