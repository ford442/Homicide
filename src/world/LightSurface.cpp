#include "world/LightSurface.hpp"
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <SDL2/SDL_gpu.h>
#include "dir.hpp"
using S = world::LightSurface;

#define ERR(msg) std::cerr << __func__ << " : " << msg << std::endl;

inline Uint32 get_pixel32(SDL_Surface *surface, int x, int y){
    Uint32 *pixels = (Uint32*)surface->pixels;
    return pixels[(y * surface->w) + x];
}

S::LightSurface(float *pixel_size, float *x, float *y, SDL_Color* ambient_color, std::shared_ptr<light::LightImageList> image_list) : _pixel_size(pixel_size), _x(x), _y(y), _ambient_color(ambient_color), _image_list(image_list){
    std::cout << "INFO :: allocating a LightSurface instance" << std::endl;
    light_target = nullptr;
    light_image = nullptr;
    light_poly_image = nullptr;
    light_poly_target = nullptr;
    world_light_image = nullptr;
}

S::~LightSurface(){
    std::cout << "INFO :: releasing memory from a LightSurface instance" << std::endl;
    if (light_image) GPU_FreeImage(light_image);
    if (light_target) GPU_FreeTarget(light_target);

    if (light_poly_image) GPU_FreeImage(light_poly_image);
    if (light_poly_target) GPU_FreeTarget(light_poly_target);

    if (world_light_image) GPU_FreeImage(world_light_image);
}

void S::clear_lights(void){
    lights.clear();
}

void S::push_light(std::shared_ptr<light::LightSource> light){
    lights.push_back(light);
}

void S::draw(GPU_Target* t){

    // polygons from light sources
    

    // images from light sources

    
    GPU_ClearRGBA(light_target, _ambient_color->r, _ambient_color->g, _ambient_color->b, _ambient_color->a);
    for (auto l : lights){
        display_light_img(light_target, l);
    }
    light_image->base_w = light_image->w * *_pixel_size;
    light_image->base_h = light_image->h * *_pixel_size;
    GPU_Rect src = {0, 0, float(light_image->base_w), float(light_image->base_h)};
    GPU_Blit(light_image, &src, t, -*_x + (light_image->w * *_pixel_size) / 2, -*_y + (light_image->h * *_pixel_size) / 2);
    
}

void S::draw_dev(GPU_Target *t){
    for (auto &l : lights){
        for (auto &p : l->vecVisibilityPolygonPoints){
            GPU_Rectangle(t, std::get<1>(p) * *_pixel_size - *_x-5, std::get<2>(p) * *_pixel_size - *_y-5, std::get<1>(p) * *_pixel_size - *_x+5, std::get<2>(p) * *_pixel_size - *_y+5, {255, 0, 0, 255});
            GPU_Line(t, l->x() * *_pixel_size - *_x, l->y() * *_pixel_size - *_y, std::get<1>(p) * *_pixel_size - *_x, std::get<2>(p) * *_pixel_size - *_y, {255, 0, 0, 255});
        }
        GPU_Circle(t, l->x() * *_pixel_size - *_x, l->y() * *_pixel_size - *_y, 30 * *_pixel_size, {255, 0, 0, 255});
    }
    draw_edges(t);
}

bool S::load(std::string file){
    if (file[1] != ':') file = RES + file;
    std::cout << "INFO :: load \"" << file << "\"" << std::endl;

    SDL_Surface* surface = IMG_Load(file.c_str());

    if (!surface){
        std::cerr << "ERROR :: IMG_Load : " << IMG_GetError()<< std::endl;
        return false;
    }

    std::cout << "INFO :: allocting sCell table, size : " << surface->w << " x " << surface->h << " : " << surface->w * surface->h << std::endl;
    world = std::make_unique<sCell[]>(surface->w * surface->h);

    for (int y=0; y<surface->h; y++){
        for (int x=0; x<surface->w; x++){
            Uint32 col = get_pixel32(surface, x, y);
            world[y * surface->w + x].exist = (col == 0x000000FF) || (col == 0xFF000000);
        }
    }

    convertTileMapToPolyMap(0, 0, surface->w, surface->h, surface->w);

    if (light_image) GPU_FreeImage(light_image);
    light_image = GPU_CreateImage(surface->w, surface->h, GPU_FORMAT_RGBA);

    GPU_SetBlendMode(light_image, GPU_BLEND_MULTIPLY);

    if (light_target) GPU_FreeTarget(light_target);
    light_target = GPU_LoadTarget(light_image);

    update_light_poly_size(surface->w, surface->h);

    light_poly_virtual_resolution(0.5);

    SDL_FreeSurface(surface);
    return true;
}


void S::convertTileMapToPolyMap(int sx, int sy, int w, int h, int pitch){
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
                        sEdge edge;
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
                        sEdge edge;
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
                        sEdge edge;
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
                        sEdge edge;
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

void S::calculateVisibilityPolygon(float ox, float oy, float radius, std::shared_ptr<light::LightSource> light){
    light->vecVisibilityPolygonPoints.clear();

    for (auto &e1 : vecEdges){

        for (int i = 0; i < 2; i++){
            
            float rdx, rdy;
            rdx = (i == 0 ? e1.sx : e1.ex) - light->x();
            rdy = (i == 0 ? e1.sy : e1.ey) - light->y();

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

                        float t2 = (rdx * (e2.sy - light->y()) + (rdy * (light->x() - e2.sx))) / (sdx * rdy - sdy * rdx);
                        float t1 = (e2.sx + sdx * t2 - light->x()) / rdx;

                        if (t1 > 0 && t2 >= 0 && t2 <= 1.0f){

                            if (t1 < min_t1){

                                min_t1 = t1;
                                min_px = light->x() + rdx * t1;
                                min_py = light->y() + rdy * t1;
                                min_ang = atan2f(min_py - light->y(), min_px - light->x());
                                bValid = true;
                            }
                        }
                    }
                }

                if(bValid) light->vecVisibilityPolygonPoints.push_back({ min_ang, min_px, min_py });
            }
        }
    }

    std::sort(
        light->vecVisibilityPolygonPoints.begin(),
        light->vecVisibilityPolygonPoints.end(),
        [&](const std::tuple<float, float, float> &t1, const std::tuple<float, float, float> &t2)
        {
            return std::get<0>(t1) < std::get<0>(t2);
        });

}

void S::display_light_poly(GPU_Target *t, std::shared_ptr<light::LightSource> light){
    switch (light->get_calculation()){

        case light::LightSource::Shadow_pylogon:
            calculateVisibilityPolygon(light->x(), light->y(), 1, light);

            if (light->vecVisibilityPolygonPoints.size() > 1){

                for (int i=0; i<light->vecVisibilityPolygonPoints.size()-1; i++){

                    float x1 = std::get<1>(light->vecVisibilityPolygonPoints[i]) * light_poly_resolution; 
                    float y1 = std::get<2>(light->vecVisibilityPolygonPoints[i]) * light_poly_resolution;
                    float x2 = std::get<1>(light->vecVisibilityPolygonPoints[i+1]) * light_poly_resolution; 
                    float y2 = std::get<2>(light->vecVisibilityPolygonPoints[i+1]) * light_poly_resolution; 

                    GPU_TriFilled(t, light->x() * light_poly_resolution, light->y() * light_poly_resolution, x1, y1, x2, y2, {255, 255, 255, 255});
                }

                float x1 = std::get<1>(light->vecVisibilityPolygonPoints[0]) * light_poly_resolution; 
                float y1 = std::get<2>(light->vecVisibilityPolygonPoints[0]) * light_poly_resolution; 
                float x2 = std::get<1>(light->vecVisibilityPolygonPoints[light->vecVisibilityPolygonPoints.size()-1]) * light_poly_resolution;
                float y2 = std::get<2>(light->vecVisibilityPolygonPoints[light->vecVisibilityPolygonPoints.size()-1]) * light_poly_resolution;

                GPU_TriFilled(t, light->x() * light_poly_resolution, light->y() * light_poly_resolution, x1, y1, x2, y2, {255, 255, 255, 255});
            }
            break;
        
        case light::LightSource::Shadow_image:
            if (!light->get_calculation_image()){
                ERR("cannot draw a null calculation image");
                break;
            }

            // GPU_CircleFilled(t, light->x() * light_poly_resolution, light->y() * light_poly_resolution, 7, {255, 255, 255, 255});
            // light->get_calculation_image()->draw(t, light->x(), light->y(), light->angle());
            break;
        
        default:
            break;
    }
}

void S::display_light_img(GPU_Target *t, std::shared_ptr<light::LightSource> light){
    if (light->is_on()) light->get_image()->draw(t, light->x(), light->y(), light->angle());
}

void S::draw_edges(GPU_Target* t){
    for (sEdge &e : vecEdges){
        int ex = (e.sx * *_pixel_size) - *_x;// + light_image->base_w;
        int ey = (e.sy * *_pixel_size) - *_y;// + light_image->base_h;
        int sx = (e.ex * *_pixel_size) - *_x;// + light_image->base_w;
        int sy = (e.ey * *_pixel_size) - *_y;// + light_image->base_h;

        GPU_Line(t, sx, sy, ex, ey, {255, 255, 255, 255});

        GPU_CircleFilled(t, ex, ey, 3, {255, 0, 0, 255});
        GPU_CircleFilled(t, sx, sy, 3, {255, 0, 0, 255});
    }
}

void S::update_light_poly_size(int w, int h){
    std::cout << "INFO :: new light poly surface size : " << w << " x " << h << std::endl;
    if (light_poly_image) GPU_FreeImage(light_poly_image);
    light_poly_image = GPU_CreateImage(w, h, GPU_FORMAT_RGBA);

    if (light_poly_target) GPU_FreeTarget(light_poly_target);
    light_poly_target = GPU_LoadTarget(light_poly_image);

    
    GPU_SetBlendMode(light_poly_image, GPU_BLEND_MULTIPLY);

    light_poly_image->w = light_image->w;
    light_poly_image->h = light_image->h;
}

void S::light_poly_virtual_resolution(const float resolution){
    light_poly_resolution = resolution;
    GPU_SetVirtualResolution(light_poly_target, float(light_image->w) * resolution, float(light_image->h) * resolution);
}

std::shared_ptr<light::LightSource> S::push_light(std::string light_name){
    std::cout << "INFO :: load light from \"" << light_name << "\" light name" << std::endl;
    std::shared_ptr<light::LightSource> light = std::make_shared<light::LightSource>();

    light->set_image(_image_list->get(light_name));
    if (!light->get_image()){
        std::cerr << "ERROR :: cannot found \"" << light_name << "\" light name" << std::endl;
        return nullptr;
    }

    lights.push_back(light);
    return light;
}

bool S::load_world_light_image(XMLNode* node){
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (!strcmp(attr.key, "path")){
            std::string path = attr.value;

            if (path[1] != ':') path = RES + path;
            SDL_Surface *surface = IMG_Load(path.c_str());

            if (!surface){
                std::cerr << "ERROR :: IMG_Load : " << IMG_GetError() << ", path : \"" << path << "\"" << std::endl;
                return false;
            }

            world_light_image = GPU_CopyImageFromSurface(surface);
            SDL_FreeSurface(surface);
        }
    }
    return true;
}

bool S::set_shader(Uint32 shader){
    int loc = GPU_GetUniformLocation(shader, "u_texture");

    if (loc != -1){
        GPU_SetShaderImage(light_poly_image, loc, 1);
        return true;
    }
    return false;
}

void S::draw_poly(GPU_Target *t){
    GPU_ClearRGBA(light_poly_target, reset_color.r, reset_color.g, reset_color.b, reset_color.a);
    for (auto l : lights){
        display_light_poly(light_poly_target, l);
    }
    light_poly_image->base_w = light_poly_image->w * *_pixel_size;
    light_poly_image->base_h = light_poly_image->h * *_pixel_size;
    GPU_Rect src = {0, 0, float(light_poly_image->base_w), float(light_poly_image->base_h)};
    GPU_Blit(light_poly_image, &src, t, -*_x + (light_poly_image->w * *_pixel_size) / 2, -*_y + (light_poly_image->h * *_pixel_size) / 2);
}

void S::copy_poly(GPU_Target *t){
    light_poly_image->base_w = light_poly_image->w * *_pixel_size;
    light_poly_image->base_h = light_poly_image->h * *_pixel_size;
    GPU_Rect src = {0, 0, float(light_poly_image->base_w), float(light_poly_image->base_h)};
    GPU_Blit(light_poly_image, &src, t, -*_x + (light_poly_image->w * *_pixel_size) / 2, -*_y + (light_poly_image->h * *_pixel_size) / 2);
}

void S::set_reset_color(SDL_Color color){
    reset_color = color;
}
