#pragma once

#include <iostream>
#include <memory>
#include "xml.hpp"

#if !defined(RAY_MAX_DIST)
    #define RAY_MAX_DIST 1000
#endif

namespace world{
    class Collisions{
        public:
            Collisions();
            ~Collisions();

            /**
             * @brief load the collision from the given xml node
             * @param node the xml node from a XMLDocument
             * @return true if succesfuly loaded, false otherwise
             */
            bool load(XMLNode *node);

            /**
             * @brief load the given image to convert it into the collisions table
             * @param image_path the path to the source image
             * @return true if loaded false ontherwise
             */
            bool load(std::string image_path);

            enum Collision_type{
                Collision_none,                                     // 0000
                Collision_enemys_shoots,                            // 0001
                Collision_enemys,                                   // 0010
                Collision_enemys_and_enemys_shoots,                 // 0011
                Collision_player_shoots,                            // 0100
                Collision_all_shoots,                               // 0101
                Collision_player_shoots_and_enemys,                 // 0110
                Collision_player_and_enemys_shoots_plus_enemys,     // 0111
                Collision_player,                                   // 1000
                Collision_player_and_enemys_choots,                 // 1001
                Collision_player_and_enemys,                        // 1010
                Collision_player_and_enemys_plus_enemys_shoots,     // 1011
                Collision_player_and_player_shoots,                 // 1100
                Collision_player_and_all_shoots,                    // 1101
                Collision_player_and_player_shoots_plus_enemys,     // 1110
                Collision_all,                                      // 1111

                // used for detection

                Collision_det_player,
                Collision_det_player_shoot,
                Collision_det_enemy,
                Collision_det_enemy_shoot
            };

            struct Colors{
                #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                    Uint32 none =                                   0x000000FF;
                    Uint32 enemys_shoots =                          0x00000FFF;
                    Uint32 enemys =                                 0x0000F0FF;
                    Uint32 enemys_and_enemys_shoots =               0x0000FFFF;
                    Uint32 player_shoots =                          0x000F00FF;
                    Uint32 all_shoots =                             0x000F0FFF;
                    Uint32 player_shoots_and_enemys =               0x000FF0FF;
                    Uint32 player_and_enemys_shoots_plus_enemys =   0x000FFFFF;
                    Uint32 player =                                 0x00F000FF;
                    Uint32 player_and_enemys_shoots =               0x00F00FFF;
                    Uint32 player_and_enemys =                      0x00F0F0FF;
                    Uint32 player_and_enemys_plus_enemys_shoots =   0x00F0FFFF;
                    Uint32 player_and_player_shoots =               0x00FF00FF;
                    Uint32 player_and_all_shoots =                  0x00FF0FFF;
                    Uint32 player_and_player_shoots_plus_enemys =   0x00FFF0FF;
                    Uint32 all =                                    0x00FFFFFF;
                #else
                    Uint32 none =                                   0xFF000000;
                    Uint32 enemys_shoots =                          0xFFF00000;
                    Uint32 enemys =                                 0xFF0F0000;
                    Uint32 enemys_and_enemys_shoots =               0xFFFF0000;
                    Uint32 player_shoots =                          0xFF00F000;
                    Uint32 all_shoots =                             0xFFF0F000;
                    Uint32 player_shoots_and_enemys =               0xFF0FF000;
                    Uint32 player_and_enemys_shoots_plus_enemys =   0xFFFFF000;
                    Uint32 player =                                 0xFF000F00;
                    Uint32 player_and_enemys_shoots =               0xFFF00F00;
                    Uint32 player_and_enemys =                      0xFF0F0F00;
                    Uint32 player_and_enemys_plus_enemys_shoots =   0xFFFF0F00;
                    Uint32 player_and_player_shoots =               0xFF00FF00;
                    Uint32 player_and_all_shoots =                  0xFFF0FF00;
                    Uint32 player_and_player_shoots_plus_enemys =   0xFF0FFF00;
                    Uint32 all =                                    0xFFFFFF00;
                #endif
            };

            /**
             * @brief get thr type of collision at the givent coordonates
             * @param x the target x
             * @param y the target y
             * @return return the type of collision
             */
            Collision_type get(const float x, const float y);

            /**
             * @brief get if the given collision type can stop the player
             * 
             * @param type the collision type
             * @return true if the collision block the player, false if not
             */
            bool is_player_collision(Collision_type type);

            /**
             * @brief get if the given collision type can stop the player projectiles
             * 
             * @param type the collision type
             * @return true if the collision block the player prjectiles, false if not
             */
            bool is_player_shoot_collision(Collision_type type);

            /**
             * @brief get if the given collision type can stop ennemys
             * 
             * @param type the collision type
             * @return true if the collision block ennemys, false if not
             */
            bool is_enemy_collision(Collision_type type);

            /**
             * @brief get if the given collision type can stop ennemys projectile
             * 
             * @param type the collision type
             * @return true if the collision block ennemys projectiles, false if not
             */
            bool is_enemy_shoot_collision(Collision_type type);

            /**
             * @brief get if the given collision type can stop projectiles
             * 
             * @param type the collision type
             * @return true if the collision block projecitles, false if not
             */
            bool is_shoot_collision(Collision_type type);

            
            enum Direction{
                dir_north,
                dir_west,
                dir_south,
                dir_east
            };

            /**
             * @brief get the distance bettween the start position and the targeting
             * 
             * @param sx the start x
             * @param sy the start y 
             * @param dir the direction 
             * @param distance the distance wher the ray stop
             * @param type the type of collision to detect
             * @return the distance bettween the start pos and the fisrt collision occured
             */
            float get_dist(const int sx, const int sy, const Direction dir, float distance, Collision_type type);

            /**
             * @brief get the type of collision from the given color
             * 
             * @param color the color as hexadecimal 0xRRGGBBAA
             * @return the type of collisons
             */
            Collision_type get_type_from_color(Uint32 color);

            /**
             * @brief get the color the given collision type
             * 
             * @param type the type to test
             * @return the color of the collision type as a hexadecimal color 0xRRGGBBAA
             */
            Uint32 get_color_from_type(Collision_type type);

            SDL_FPoint cast_ray(float x1, float y1, float x2, float y2);
            float cast_ray(float x, float y, const float angle);

        private:
            int world_w, world_h;
            std::unique_ptr<Collision_type[]> _world;

            Colors colors;

            bool is_collision(int x, int y, Collision_type type);
    };
}
