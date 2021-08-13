#include "lights/ShadowCaster.hpp"
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <SDL2/SDL_gpu.h>
#include "dir.hpp"
#include "main.hpp"

// return the color of the pixel as an hexadecimal color 0xRRGGBBAA
static inline Uint32 get_pixel32(SDL_Surface *surface, int x, int y){
    Uint32 *pixels = (Uint32*)surface->pixels;
    return pixels[(y * surface->w) + x];
}

ShadowCaster::ShadowCaster(){
    CONSTRUCT()
}

ShadowCaster::~ShadowCaster(){
    DESTRUCT();
}

// load the shadow caster from the secified file
bool ShadowCaster::load(std::string file){
    if (file[1] != ':') file = RES + file;
    LOAD_LOG(file);

    SDL_Surface* surface = IMG_Load(file.c_str());
    if (!surface){
        ERR("IMG_Load : " + std::string(IMG_GetError()));
        return false;
    }

    ALLOC_LOG("Cell table : " + std::to_string(surface->w * surface->h));
    world = std::make_unique<Cell[]>(surface->w * surface->h);

    for (int y=0; y<surface->h; y++){
        for (int x=0; x<surface->w; x++){
            Uint32 col = get_pixel32(surface, x, y);
            world[y * surface->w + x].exist = (col == 0x000000FF) || (col == 0xFF000000);
        }
    }

    convertTileMapToPolyMap(0, 0, surface->w, surface->h, surface->w);
    SDL_FreeSurface(surface);

    return true;
}


void ShadowCaster::convertTileMapToPolyMap(int sx, int sy, int w, int h, int pitch){
    std::cout << "INFO :: convert light walls (tile map) into polygon map (vec)" << std::endl;
    vecEdges.clear();

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            for (int j = 0; j < 4; j++)
            {
                world[(y + sy) * pitch + (x + sx)].edge_exist[j] = false;
                world[(y + sy) * pitch + (x + sx)].edge_id[j] = 0;
            }

    for (int x = 1; x < w - 1; x++){
        for (int y = 1; y < h - 1; y++)
        {
            int i = (y + sy) * pitch + (x + sx);
            int n = (y + sy - 1) * pitch + (x + sx);
            int s = (y + sy + 1) * pitch + (x + sx);
            int w = (y + sy) * pitch + (x + sx - 1);
            int e = (y + sy) * pitch + (x + sx + 1);

            if (world[i].exist)
            {
                if (!world[w].exist)
                {
                    if (world[n].edge_exist[WEST])
                    {
                        vecEdges[world[n].edge_id[WEST]].ey += 1;
                        world[i].edge_id[WEST] = world[n].edge_id[WEST];
                        world[i].edge_exist[WEST] = true;
                    }
                    else
                    {
                        Edge edge;
                        edge.sx = (sx + x) * 1; edge.sy = (sy + y) * 1;
                        edge.ex = edge.sx; edge.ey = edge.sy + 1;

                        int edge_id = vecEdges.size();
                        vecEdges.push_back(edge);

                        world[i].edge_id[WEST] = edge_id;
                        world[i].edge_exist[WEST] = true;
                    }
                }

                if (!world[e].exist)
                {
                    if (world[n].edge_exist[EAST])
                    {
                        vecEdges[world[n].edge_id[EAST]].ey += 1;
                        world[i].edge_id[EAST] = world[n].edge_id[EAST];
                        world[i].edge_exist[EAST] = true;
                    }
                    else
                    {
                        Edge edge;
                        edge.sx = (sx + x + 1) * 1; edge.sy = (sy + y) * 1;
                        edge.ex = edge.sx; edge.ey = edge.sy + 1;

                        int edge_id = vecEdges.size();
                        vecEdges.push_back(edge);

                        world[i].edge_id[EAST] = edge_id;
                        world[i].edge_exist[EAST] = true;
                    }
                }

                if (!world[n].exist){
                    if (world[w].edge_exist[NORTH]){

                        vecEdges[world[w].edge_id[NORTH]].ex += 1;
                        world[i].edge_id[NORTH] = world[w].edge_id[NORTH];
                        world[i].edge_exist[NORTH] = true;

                    } else {
                        Edge edge;
                        edge.sx = (sx + x) * 1; edge.sy = (sy + y) * 1;
                        edge.ex = edge.sx + 1; edge.ey = edge.sy;

                        int edge_id = vecEdges.size();
                        vecEdges.push_back(edge);

                        world[i].edge_id[NORTH] = edge_id;
                        world[i].edge_exist[NORTH] = true;
                    }
                }

                if (!world[s].exist){
                    if (world[w].edge_exist[SOUTH]){
                        vecEdges[world[w].edge_id[SOUTH]].ex += 1;
                        world[i].edge_id[SOUTH] = world[w].edge_id[SOUTH];
                        world[i].edge_exist[SOUTH] = true;

                    } else {
                        Edge edge;
                        edge.sx = (sx + x) * 1; edge.sy = (sy + y + 1) * 1;
                        edge.ex = edge.sx + 1; edge.ey = edge.sy;

                        int edge_id = vecEdges.size();
                        vecEdges.push_back(edge);

                        world[i].edge_id[SOUTH] = edge_id;
                        world[i].edge_exist[SOUTH] = true;
                    }
                }
            }
        }
    }
}

void ShadowCaster::calculateVisibilityPolygon(float ox, float oy, float radius, std::vector<Visibility_poly_point> points, float w, float h){
    points.clear();

    w /= 2;
    h /= 2;

    const float left = ox - w;
    const float right = ox + w;
    const float top = oy - h;
    const float down = oy - h;

    vecEdges.push_back({right, down, right, top}); // right border
    vecEdges.push_back({right, top, left, top}); // top border
    vecEdges.push_back({left, top, left, down}); // left border
    vecEdges.push_back({left, down, right, down}); // down border

    for (auto &e1 : vecEdges){
        
        // pass useless edges
        if (e1.sx > right && e1.ex > right) continue;
        if (e1.sx < left && e1.ex < left) continue;
        if (e1.sy > down && e1.ey > down) continue;
        if (e1.sy < top && e1.ey < top) continue;

        for (int i = 0; i < 2; i++){
            
            float rdx, rdy;
            rdx = (i == 0 ? e1.sx : e1.ex) - ox;
            rdy = (i == 0 ? e1.sy : e1.ey) - oy;

            float base_ang = atan2f(rdy, rdx);

            float ang = 0;
            for (int j = 0; j < 3; j++)
            {
                if (j == 0)	ang = base_ang - 0.0001f;
                if (j == 1)	ang = base_ang;
                if (j == 2)	ang = base_ang + 0.0001f;

                rdx = radius * cosf(ang);
                rdy = radius * sinf(ang);

                float min_t1 = INFINITY;
                float min_px = 0, min_py = 0, min_ang = 0;
                bool bValid = false;

                for (auto &e2 : vecEdges){

                    float sdx = e2.ex - e2.sx;
                    float sdy = e2.ey - e2.sy;

                    if (fabs(sdx - rdx) > 0.0f && fabs(sdy - rdy) > 0.0f){

                        float t2 = (rdx * (e2.sy - oy) + (rdy * (ox - e2.sx))) / (sdx * rdy - sdy * rdx);
                        float t1 = (e2.sx + sdx * t2 - ox) / rdx;

                        if (t1 > 0 && t2 >= 0 && t2 <= 1.0f){

                            if (t1 < min_t1){

                                min_t1 = t1;
                                min_px = ox + rdx * t1;
                                min_py = oy + rdy * t1;
                                min_ang = atan2f(min_py - oy, min_px - ox);
                                bValid = true;
                            }
                        }
                    }
                }

                if(bValid) points.push_back({min_px, min_py, min_ang});
            }
        }
    }

    std::sort(
        points.begin(),
        points.end(),
        [&](const Visibility_poly_point &t1, const Visibility_poly_point &t2){
            return t1.min_angle < t2.min_angle;
        });
    
    // supress added edges
    for (int i=0; i<4; i++){
        vecEdges.pop_back();
    }

}

bool ShadowCaster::load(XMLNode *node){

    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "path")){
            return load(attr.value);
        }
    }

    return false;
}


void ShadowCaster::calculate(const int x, const int y, std::vector<Visibility_poly_point> points, const float w, const float h){
    calculateVisibilityPolygon(x, y, 1, points, w, h);
}


std::vector<ShadowCaster::Edge> ShadowCaster::get_edges(void) const{
    return vecEdges;
}