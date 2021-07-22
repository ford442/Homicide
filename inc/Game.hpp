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
#include "weapons/Weapon_list.hpp"
#include "csv.hpp"



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

        bool Init_with_csv(std::string path);

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

        bool load_world(std::string world_path);

    private:

        bool launched;

        GPU_Target* _target;
        SDL_Window* _window;

        void event(void);
        void draw(void);
        void update(void);
        void delay(void);

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

        std::shared_ptr<entity::Player> _player;

        std::shared_ptr<Camera> _camera;

        Uint32 blur_vert;
        Uint32 blur_frag;
        Uint32 blur_shader;

        GPU_ShaderBlock blur;

        int blur_radius;
        int blur_resolution;
        int blur_dir;

        CSV::Document settings_file;
        std::string _version;

        void set_version(std::string version);
};

#ifndef __GAME__HPP__
    #define __GAME__HPP__

    #include <iostream>
    #include <SDL2/SDL_gpu.h>

    #include <SDL/Window.hpp>

    class Game{
        public:
            Game();
            ~Game();

            bool load_setting_xml(std::string path);
            bool load_setting_csv(std::string path);

            struct Bloom_shared{
                Uint32 bloom_vert;
                Uint32 bloom_frag;
                Uint32 bloom_shader;

                GPU_ShaderBlock bloom_block;
            };
        
        private:

            // SDL
            SDL::Window *_window;

            world::World _world;
            event::Handler _event_handler;
            sprite::Animations _animations;
            entity::EntityList _entityList;
            light::LightImageList _lightImageList;
            weapons::Weapon_list _weapon_list;
            Camera _camera;

            entity::Player _player;

            Bloom_shared _bloom;

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
    };

#endif