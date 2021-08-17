#ifndef __SHADOWCASTER__HPP__
    #define __SHADOWCASTER__HPP__

    #include <iostream>
    #include <memory>
    #include <vector>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_gpu.h>

    #include "xml.hpp"

    class ShadowCaster{
        public:
            ShadowCaster();
            ~ShadowCaster();

            /**
             * @brief the struct where data of visibility polygone from light source are stored
             * 
             */
            struct Visibility_poly_point{
                float x, y;
                float min_angle;
            };
            
            /**
             * @brief load the shadow caster from the given xml node
             * @param node the xml node form a xml document
             * @return true if succesfuly loaded, false on error
             */
            bool load(XMLNode *node);

            /**
             * @brief load the given xml file
             * 
             * @param file the path to the xml file (relative to the resources folder)
             * @return true if succesfuly loaded, false on error
             */
            bool load(std::string file);

            /**
             * @brief calculate the visibility polygon from the given position
             * 
             * @param x the observator x
             * @param y the observator y
             * @param points the vector where all points will be pushed
             */
            void calculate(const int x, const int y, std::vector<Visibility_poly_point> points, const float w, const float h);

            struct Edge{
                float sx, sy;
                float ex, ey;
            };

            /**
             * @brief get the vector of edges
             * @return std::vector<Edge> 
             */
            std::vector<Edge>* get_edges(void);
            
        private:

            /**
             * @brief transform the Cell world into vertical and horizontal egdes
             * 
             * @param sx the start location in the world
             * @param sy the end location in the world
             * @param w the width of the region to calculate
             * @param h the height of the region to calculate
             * @param pitch the position calculation coefition (y * pitch + x)
             */
            void convertTileMapToPolyMap(int sx, int sy, int w, int h, int pitch);

            /**
             * @brief calculate the visility from the obsevator position 
             * 
             * @param ox the observator x
             * @param oy the observator y
             * @param radius the radius of calculation
             * @param points the vector where visibility points will be pushed
             */
            void calculateVisibilityPolygon(float ox, float oy, float radius, std::vector<Visibility_poly_point> points, float w, float h);

            struct Cell{
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

            std::unique_ptr<Cell[]> world;
            std::vector<Edge> vecEdges;
    };
#endif