#pragma once

#include <iostream>
#include <memory>
#include <SDL2/SDL_gpu.h>

#include "events/Event_handler.hpp"
#include "world/World.hpp"
#include "lights/LightImageList.hpp"
#include "sprites/Animations.hpp"

#include "entity/EntityList.hpp"
#include "entity/Player.hpp"
#include "entity/NPC.hpp"
#include "Camera.hpp"
#include "weapons/Weapon_type_list.hpp"
#include "weapons/Projectile_type.hpp"
#include "weapons/Projectile.hpp"

class Game{
    public:
        Game();
        ~Game();

        // initialization
        bool Init_Window(void);
        bool Init_TTF(void);
        bool Init_Mixer(void);
        bool Init_IMG(void);
        bool Init_libs(void);
        bool Init_logs(void);

        void run(void);
        void quit(void);

        // fps
        void set_max_fps(int fps);

        // display
        float get_pixel_size(void) const;
        SDL_Window* get_window(void) const;
        GPU_Target* get_target(void) const;

        float get_x(void) const;
        float get_y(void) const;

        bool load_settings_file(std::string path);
        bool load_world(std::string world_path);

    private:

        bool launched;

        GPU_Target* _target;
        SDL_Window* _window;

        void event(void);
        void draw(void);
        void update(void);
        void delay(void);

        void set_window_title(std::string title);
        void set_window_width(int w);
        void set_window_height(int h);
        void set_window_min_size(int w, int h);
        void set_window_max_size(int w, int h);
        void set_target_width(int w);
        void set_target_height(int h);

        bool load_lights(std::string dir_path);
        bool load_projectiles(std::string dir_path);
        bool load_animations(std::string dir_path);
        bool load_weapons(std::string dir_path);

        void shoot(int x, int y, int dir);

        // ticks
        int start_tick;
        int delta_tick;
        int execution_tick;
        int max_ticks_per_frames;
        int fps;
        int fps_counter;
        int fps_tick;

        // display attributes
        float _pixel_size;
        float _x, _y;
        int window_w, window_h;
        bool debug_mod;

        std::shared_ptr<event::Handler> _event_handler;
        std::shared_ptr<world::World> _world;
        std::shared_ptr<sprite::Animations> _animations;
        std::shared_ptr<entity::EntityList> _entityList;
        std::shared_ptr<light::LightImageList> _lightImageList;
        std::shared_ptr<weapons::Weapon_list> _weapon_list;

        std::list<std::shared_ptr<Projectile_type>> projectile_types;
        std::list<std::shared_ptr<Projectile>> projectiles;

        std::shared_ptr<entity::Player> _player;

        Camera _camera;

        Uint32 blur_vert;
        Uint32 blur_frag;
        Uint32 blur_shader;

        GPU_ShaderBlock blur;

        int blur_radius;
        int blur_resolution;
        int blur_dir;

        world::Floor world_floor;
        world::Top world_top;
        world::A_star Astar;
        world::Collisions collisons;
        world::LightSurface light_layer;
};