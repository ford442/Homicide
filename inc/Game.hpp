#pragma once

#include <iostream>
#include <memory>
#include <list>
#include <SDL2/SDL_gpu.h>
#include "events/Event_handler.hpp"
#include "lights/ShadowCaster.hpp"
#include "world/A-star.hpp"
#include "world/Collisions.hpp"
#include "world/Floor.hpp"
#include "world/Top.hpp"
#include "Camera.hpp"
#include "shaders/Blur.hpp"
#include "lights/Light.hpp"
#include "lights/LightSource.hpp"
#include "widgets/Widget.hpp"
#include "SDL/TTF.hpp"
#include "entity/Entity.hpp"
#include "sprites/SpriteSheet.hpp"

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
        bool load_spriteSheets(std::string dir_path);
        bool load_spritesheet(std::string file);
        bool load_weapons(std::string dir_path);
        bool load_world_floor(XMLNode *node);
        bool load_world_top(XMLNode *node);
        bool load_text_widget(XMLNode *node);
        bool load_rect_widget(XMLNode *node);
        bool load_border_widget(XMLNode *node);
        bool load_textButton_widget(XMLNode *node);
        bool load_boolTextButton_widget(XMLNode *node);

        bool is_floor_loaded;
        bool is_top_loaded;
        bool is_Astar_loaded;
        bool is_collisions_loaded;

        bool is_everything_loaded(void) const;

        void shoot(int x, int y, int dir);

        bool blit_floor(void);
        bool blit_top(void);
        void update_widgets(void);
        void blit_widgets(void);
        void blit_widgets_HUD(void);
        void update_cam_events(void);
        void update_entitys(void);
        void blit_entitys(void);

        bool load_save(std::string path);
        void reset_keys(void);
        bool load_menu(std::string path);

        bool *get_value(std::string value_name);
        bool *get_value_widget(std::string value);
        bool *get_value_shadowCaster(std::string value);
        bool *get_value_camera(std::string value);

        // ticks
        int start_tick;
        int delta_tick;
        int execution_tick;
        int max_ticks_per_frames;
        int fps;
        int fps_counter;
        int fps_tick;

        // display attributes
        float _zoom, tzoom;
        float _x, _y;
        float tx, ty;
        int window_w, window_h;
        bool debug_mod;
        
        std::list<std::shared_ptr<sprite::SpriteSheet>> spriteSheets;
        std::list<std::shared_ptr<Entity>> entitys;

        Camera _camera;
        SDL_Scancode cam_up_key;
        SDL_Scancode cam_down_key;
        SDL_Scancode cam_left_key;
        SDL_Scancode cam_right_key;
        SDL_Scancode cam_zoom_in_key;
        SDL_Scancode cam_zoom_out_key;
        
        event::Handler events;
        world::Floor world_floor;
        world::Top world_top;
        world::A_star Astar;
        world::Collisions collisions;
        ShadowCaster shadowCaster;
        Light light_layer;
        TTF fonts;

        // shader
        shader::Blur blur;

        // menus
        std::string curr_menu;
        std::string pause_menu_path;
        SDL_Scancode pause_key;
        std::string debug_menu_path;
        SDL_Scancode debug_key;

        bool is_menu_opened;

        std::list<std::shared_ptr<Widget>> widgets;

        // debug
        bool render_widget_border;
        bool render_hovered_widget_border;
        bool render_shadowCaster_borders;
        bool free_camera;
};