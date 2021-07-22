#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>

#include "xml.hpp"

#include "lights/LightImageList.hpp"
#include "lights/LightImage.hpp"
#include "lights/LightSource.hpp"

namespace world{
    class LightSurface{
        public:
            LightSurface(float *pixel_size, float *x, float *y, SDL_Color* ambient_light, std::shared_ptr<light::LightImageList> image_list);
            ~LightSurface();

            void clear_lights(void);
            void push_light(std::shared_ptr<light::LightSource> light);
            std::shared_ptr<light::LightSource> push_light(std::string light_name);
            std::vector<std::shared_ptr<light::LightSource>>* get_lights(void);

            void draw(GPU_Target* t);
            void draw_poly(GPU_Target *t);
            void copy_poly(GPU_Target *t);
            void draw_dev(GPU_Target *t);
            
            bool load(std::string file);

            void draw_edges(GPU_Target* t);

            void update_light_poly_size(int w, int h);

            // from 0 to 1
            void light_poly_virtual_resolution(const float resolution);

            bool load_world_light_image(XMLNode* node);

            bool set_shader(Uint32 shader);

            void set_reset_color(SDL_Color color);
        
        private:

            GPU_Image* light_image;
            GPU_Target* light_target;

            GPU_Image* light_poly_image;
            GPU_Target* light_poly_target;

            GPU_Image* world_light_image;

            float *_x;
            float *_y;
            float *_pixel_size;
            SDL_Color* _ambient_color;
            std::shared_ptr<light::LightImageList> _image_list;

            std::vector<std::shared_ptr<light::LightSource>> lights;

            struct sEdge{
                float sx, sy;
                float ex, ey;
            };

            struct sCell{
                int edge_id[4];
                bool edge_exist[4];
                bool exist = false;
            };

            enum Egde_ID{
                NORTH = 0,
                SOUTH = 1,
                EAST = 2,
                WEST = 3
            };

            std::unique_ptr<sCell[]> world;
            std::vector<sEdge> vecEdges;
            std::vector<std::tuple<float, float, float>> vecVisibilityPolygonPoints;

            void convertTileMapToPolyMap(int sx, int sy, int w, int h, int pitch);
            void calculateVisibilityPolygon(float ox, float oy, float radius, std::shared_ptr<light::LightSource> light);

            void display_light_poly(GPU_Target *t, std::shared_ptr<light::LightSource> light);
            void display_light_img(GPU_Target *t, std::shared_ptr<light::LightSource> light);

            float light_poly_resolution;

            SDL_Color reset_color;

    };
}