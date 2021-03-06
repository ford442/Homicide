#include "Game.hpp"
#include "dir.hpp"
#include <filesystem>
#include "csv.hpp"
#include "main.hpp"
#include "widgets/Text.hpp"
#include "widgets/Rect.hpp"
#include "widgets/Border.hpp"
#include "widgets/TextButton.hpp"
#include "widgets/BoolTextButton.hpp"

using G = Game;

int InitSDL_SUB_Libs(void *ptr);

G::Game(void){
    launched = false;
    _window = nullptr;
    render_widget_border = false;
    render_hovered_widget_border = false;
    render_shadowCaster_borders = false;
    delta_tick = 0;
    tzoom = 1;
    tx = 0;
    ty = 0;
    reset_keys();
}

G::~Game(){
    
}

bool G::Init_Window(void){
    std::cout << "GPU_Init()" << std::endl;
    window_w = 920;
    window_h = 540;
    _window = SDL_CreateWindow("Homicide", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    
    if (!_window){
        std::cerr << "ERROR :: SDL_CreateWindow : " << SDL_GetError() << std::endl;
        return false;
    }

    GPU_SetInitWindow(SDL_GetWindowID(_window));

    _target = GPU_Init(window_w, window_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!_target){
        return false;
    }

    GPU_Clear(_target);
    GPU_Flip(_target);
    return true;
}

bool G::Init_TTF(void){
    return fonts.init();
}

bool G::Init_Mixer(void){
    std::cout << "Init_Mixer()" << std::endl;
    if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_MOD | MIX_INIT_OGG | MIX_INIT_FLAC)){
        std::cerr << "ERROR :: MIX_Init : " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

bool G::Init_libs(void){

    GPU_SetDebugLevel(GPU_DEBUG_LEVEL_3);
    std::cout << "INFO :: Init librarys" << std::endl;
    Init_Window();
    bool error = InitSDL_SUB_Libs(this);
    std::cout << "INFO :: SDL2 libs initalized" << std::endl;


    _camera.set_delay(350);
    _camera.setMovementType(Camera::Camera_FadeIn);
    debug_mod = false;

    std::string vert_filename(RES + "shaders\\blur\\vert.glsl");
    std::string frag_filename(RES + "shaders\\blur\\frag.glsl");
    
    blur.load(vert_filename, frag_filename);

    _x = 0;
    _y = 0;

    set_max_fps(60);
    launched = true;

    std::cout << std::endl << "INFO :: game launched success : " << (launched == true ? "true" : "false") << " ticks : " << SDL_GetTicks() << " milisecond" << std::endl << std::endl;

    if (!load_settings_file("data\\options.csv")) return false;
    SDL_GetWindowSize(_window, &window_w, &window_h);

    _zoom = window_w / 450;
    tzoom = window_w / 450;
    if (!load_save("data\\worlds\\menu_map.xml")) return false;

    std::cout << "INFO :: game initialization ended" << std::endl;
    return error;
}

void G::run(void){
    while (launched){
        start_tick = SDL_GetTicks();

        event();
        update();
        draw();
        delay();
    }
}

void G::event(void){
    SDL_Event e;

    while (SDL_PollEvent(&e)){
        switch (e.type){
            case SDL_QUIT:
                launched = false;
                return;
            
            case SDL_WINDOWEVENT:
				switch (e.window.event){
                    case SDL_WINDOWEVENT_RESIZED:
                        GPU_SetWindowResolution(e.window.data1, e.window.data2);
                        _zoom = e.window.data1 / 450;
                        window_w = e.window.data1;
                        window_h = e.window.data2;
                        load_menu(curr_menu);
                        break;

                    default:
                        break;
				}
				break;
            
            default:
                break;
        }

        events.event(&e);
    }
}

void G::draw(void){
    GPU_Clear(_target);
    
    // blur.active();
    blit_floor();
    blit_entitys();

    blit_top();

    blit_lightSources_poly();
    if (render_shadowCaster_borders){
        for (ShadowCaster::Edge &e : *shadowCaster.get_edges()){
            int ex = (e.sx * _zoom) - _x;
            int ey = (e.sy * _zoom) - _y;
            int sx = (e.ex * _zoom) - _x;
            int sy = (e.ey * _zoom) - _y;

            GPU_Line(_target, sx, sy, ex, ey, {255, 255, 255, 255});

            GPU_CircleFilled(_target, ex, ey, 3, {255, 0, 0, 255});
            GPU_CircleFilled(_target, sx, sy, 3, {255, 0, 0, 255});
        }
    }

    blit_widgets();
    // blur.unactive();

    GPU_Flip(_target);
}

void Game::shoot(int x, int y, int dir){

}

void Game::update(void){

    // pause menu
    if (events.IsKeyPush(pause_key)){
        if (is_menu_opened){
            widgets.clear();
            is_menu_opened = false;
            curr_menu.clear();
        } else {
            is_menu_opened = true;
            load_menu(pause_menu_path);
        }
    } else if (events.IsKeyPush(debug_key)){
        if (is_menu_opened){
            widgets.clear();
            is_menu_opened = false;
            curr_menu.clear();
        } else {
            is_menu_opened = true;
            load_menu(debug_menu_path);
        }
    } else if (events.IsKeyPush(SDL_SCANCODE_F5)){
        load_menu(curr_menu);
    }

    

    update_cam_events();
    update_widgets();
    
    float x, y;
    _camera.get_pos(&x, &y);
    _x = (x * _zoom) - window_w / 2 + (events.mouse_x() - (window_w / 2)) / 10;
    _y = (y * _zoom) - window_h / 2 + (events.mouse_y() - (window_h / 2)) / 10;

    if (!is_menu_opened){
        
        if (events.isButtonPush(event::Mouse_button_left)){
            auto l = std::make_shared<light::LightSource>();
            l->pos(events.mouse_x(), events.mouse_y());
            l->update_size(200, 200);
            l->on();
            lightSources.push_back(l);
        }

        update_entitys();

        _camera.OnTick(delta_tick);
        _camera.go_to(tx, ty);
    }

    events.update();
}

void G::delay(void){
    fps_counter++;
    if (SDL_GetTicks() - fps_tick >= 1000){
        fps_tick = SDL_GetTicks();
        fps = fps_counter;
        fps_counter = 0;
    }

    int finnalTick = SDL_GetTicks();
    execution_tick = (finnalTick - start_tick) > max_ticks_per_frames ? max_ticks_per_frames : (finnalTick - start_tick);
    SDL_Delay(max_ticks_per_frames - execution_tick);
    delta_tick = SDL_GetTicks() - start_tick;
}

void G::set_max_fps(int fps){
    max_ticks_per_frames = 1000 / fps;
}

SDL_Window* G::get_window(void) const{
    return _window;
}

GPU_Target* G::get_target(void) const{
    return _target;
}

float G::get_x(void) const{
    return _x;
}

float G::get_y(void) const{
    return _y;
}

bool G::Init_IMG(void){
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP) == 0){
        std::cerr << "IMG_Init : " << IMG_GetError() << std::endl;
        return false;
    }
    return true;
}

void G::quit(void){
    std::cout << std::endl;
    int tick = SDL_GetTicks();
    std::cout << "INFO :: releasing memory from a Game instance" << std::endl;
    if (_window) SDL_DestroyWindow(_window);
    if (_target) GPU_FreeTarget(_target);

    std::cout << "INFO :: quit SDL2 libs" << std::endl;

    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    tick = SDL_GetTicks() - tick;

    GPU_Quit();

    std::cout << std::endl << "INFO :: Game memory release finish, release execution tick : " << tick << " milisecond" << std::endl << std::endl;
}

int InitSDL_SUB_Libs(void *ptr){
    Game *g = (Game*)ptr;

    if (!g->Init_Mixer()) return false;
    if (!g->Init_TTF()) return false;
    if (!g->Init_IMG()) return false;
    return true;
}

bool Game::load_projectiles(std::string dir_path){
    if (dir_path[1] != ':') dir_path = RES + dir_path;

    std::vector<std::string> dir_content = dir::content(dir_path);

    if (dir_content.empty()){
        ERR("directory \"" + std::string(dir_path) + "\" not found or empty");
        return false;
    }

    for (auto &f : dir_content){
        if (f == ".." || f == ".") continue;

        // auto projectile = std::make_shared<Projectile_type>();

        // if (projectile->load(dir_path + f, _lightImageList)){
        //     projectile_types.push_back(projectile);
        // }
    }
    return true;
}

bool to_bool(std::string var){
    if (var == "true") return true;
    if (var == "false") return false;

    WARN("cannot convert \"" + var + "\" into boolean, convert as false");
    return false;
}

static inline void load_key(std::string value, SDL_Scancode *_key){
    if (!value.empty()){
        SDL_Scancode key = SDL_GetScancodeFromName(value.c_str());
        if (key != SDL_SCANCODE_UNKNOWN){
            *_key = key;
        } else {
            WARN("cannot reconize \"" + value + "\" key as a key");
        }
    } else {
        ERR("cannot found node");
    }
}

bool Game::load_settings_file(std::string path){
    if (path[1] != ':') path = RES + path;
    CSV::Document doc;

    if (!doc.load(path)) return false;

    set_window_title(doc.search("window_title").c_str());

    try{
        set_window_width(std::stoi(doc.search("window_width")));
    } catch (std::exception &e) {
        ERR("standart exception : " + std::string(e.what()) + "; cannot read the window width, set to 1080");
        set_window_width(1080);
    }

    try{
        set_window_height(std::stoi(doc.search("window_height")));
    } catch (std::exception &e) {
        ERR("standart exception : " + std::string(e.what()) + "; cannot read the window height, set to 720");
        set_window_height(720);
    }

    bool fullscreen = to_bool(doc.search("window_fullscreen"));
    
    if (_window){
        if (fullscreen){
            GPU_SetFullscreen(true, false);
        } else {
            GPU_SetFullscreen(false, false);
        }

        SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        
    } else {
        ERR("cannot set the fullscreen state of a null window");
    }

    pause_menu_path = doc.search("pause_path");
    if (pause_menu_path.empty()){
        ERR("cannot found the pause path");
        return false;
    }

    std::string value = doc.search("pause_key");
    if (!value.empty()){
        SDL_Scancode key = SDL_GetScancodeFromName(value.c_str());
        if (key != SDL_SCANCODE_UNKNOWN){
            pause_key = key;
        } else {
            WARN("cannot reconize \"" + value + "\" key at \"pause_key\" declaration, set as escape");
        }
    }

    debug_menu_path = doc.search("debug_path");
    if (pause_menu_path.empty()){
        ERR("cannot found the debug path");
        return false;
    }

    load_key(doc.search("camera_up_key"), &cam_up_key);
    load_key(doc.search("camera_down_key"), &cam_down_key);
    load_key(doc.search("camera_left_key"), &cam_left_key);
    load_key(doc.search("camera_right_key"), &cam_right_key);
    load_key(doc.search("camera_zoom_in_key"), &cam_zoom_in_key);
    load_key(doc.search("camera_zoom_out_key"), &cam_zoom_out_key);
    load_key(doc.search("debug_key"), &debug_key);

    if (!fonts.load_font_dir(doc.search("fonts"))) return false;
    if (!load_lights(doc.search("lights"))) return false;
    if (!load_projectiles(doc.search("projectiles"))) return false;
    if (!load_spriteSheets(doc.search("animations"))) return false;
    if (!load_weapons(doc.search("weapons"))) return false;

    std::cout << std::endl << "settings file loaded" << std::endl << std::endl;

    return true;
}

void Game::set_window_title(std::string title){
    if (!_window){
        ERR("cannot set the title of a null window");
        return;
    }

    SDL_SetWindowTitle(_window, title.c_str());
}

void Game::set_window_width(int w){
    if (!_window){
        ERR("cannot set the title of a null window");
        return;
    }
    int h;
    SDL_GetWindowSize(_window, nullptr, &h);
    GPU_SetWindowResolution(w, h);
    window_w = w;
}

void Game::set_window_height(int h){
    if (!_window){
        ERR("cannot set the title of a null window");
        return;
    }
    int w;
    SDL_GetWindowSize(_window, &w, nullptr);
    GPU_SetWindowResolution(w, h);
    window_h = h;
}


void Game::set_window_min_size(int w, int h){
    if (!_window){
        ERR("cannot set the minimal size of a null window");
        return;
    }

    SDL_SetWindowMinimumSize(_window, w, h);
}

void Game::set_window_max_size(int w, int h){
    if (!_window){
        ERR("cannot set the minimal size of a null window");
        return;
    }
    
    SDL_SetWindowMaximumSize(_window, w, h);
}

bool Game::load_lights(std::string dir_path){
    if (dir_path[1] != ':') dir_path = RES + dir_path;
    std::vector<std::string> dir_content = dir::content(dir_path);

    if (dir_content.empty()){
        ERR("directory \"" + std::string(dir_path) + "\" not found or empty");
        return false;
    }

    for (auto &f : dir_content){
        if (f == ".." || f == ".") continue;

        // auto light = std::make_shared<light::LightImage>(&_zoom, &_x, &_y);

        // if (light->load_csv(dir_path + f)){
        //     _lightImageList->push(light);
        // }
    }
    return true;
}

bool Game::load_spriteSheets(std::string dir_path){
    if (dir_path[1] != ':') dir_path = RES + dir_path;
    std::vector<std::string> dir_content = dir::content(dir_path);

    if (dir_content.empty()){
        ERR("directory \"" + std::string(dir_path) + "\" not found");
        return false;
    }

    for (auto &f : dir_content){
        if (f == ".." || f == ".") continue;
        load_spritesheet(dir_path + f);
    }

    if (spriteSheets.empty()){
        ERR("the given path for animation \"" + dir_path + "\" is empty and/or has non-valid animations");
        return false;
    }

    return true;
}

bool Game::load_spritesheet(std::string file){
    auto sprite = std::make_shared<sprite::SpriteSheet>();

    if (sprite->load_csv(file)){
        spriteSheets.push_back(sprite);
        return true;
    }

    return false;
}

bool Game::load_weapons(std::string dir_path){
    // return _weapon_list->load(dir_path);
    return true;
}

static inline std::string get_attr(std::string key, XMLNode *node){
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (attr.key == key){
            return attr.value;
        }
    }

    return "";
}

bool Game::load_save(std::string path){
    if (path[1] != ':') path = RES + path;
    LOAD_LOG(path);

    bool err = false;
    is_menu_opened = false;
    entitys.clear();

    GPU_Clear(_target);

    XMLDocument doc;
    if (XMLDocument_load(&doc, path.c_str())){
        for (int c=0; c<doc.root->children.size; c++){
            XMLNode *child = XMLNode_child(doc.root, c);

            if (is_equal(child->tag, "floor")){
                is_floor_loaded = load_world_floor(child);
            
            } else if (is_equal(child->tag, "top")){
                is_top_loaded = load_world_top(child);
            
            } else if (is_equal(child->tag, "shadowCaster")){
                if (!shadowCaster.load(child)){
                    err = true;
                    break;
                }
            
            } else if (is_equal(child->tag, "light_layer")){
                if (!light_layer.load(child)){
                    err = true;
                    break;
                }
            
            } else if (is_equal(child->tag, "collisions")){
                if (!collisions.load(child)){
                    err = true;
                    break;
                }
        
            } else if (is_equal(child->tag, "Astar")){
                if (!Astar.load(child, &collisions)){
                    err = true;
                    break;
                }
            
            } else if (is_equal(child->tag, "entity")){
                // temporary condition, will be supressed when npc will be finished
                auto entity = std::make_shared<Entity>();
                entity->set_collisions(&collisions);

                if (entity->load(child, spriteSheets, &shadowCaster)){
                    entitys.push_back(entity);
                } else {
                    err = true;
                    break;
                }

            } else {
                WARN("cannot reconize \"" + std::string(child->tag) + "\" world xml node");
            }
            
            GPU_Clear(_target);
            GPU_SectorFilled(_target, window_w / 2, window_h / 2, window_w / 11, window_w / 10, 0, float(c) / float(doc.root->children.size) * 360, {255, 0, 0, 255});
            GPU_Flip(_target);
        }
    } else {
        return false;
    }

    XMLDocument_free(&doc);
    if (err) return false;
    update();
    return true;
}

bool Game::load_world_floor(XMLNode *node){
    std::string path = get_attr("path", node);

    if (path.empty()){
        ERR("cannot find \"" + path + "\" floor attribute");
        return false;
    }

    world_floor.scale(get_attr("scale", node));
    return world_floor.load(path);
}

bool Game::load_world_top(XMLNode *node){
    std::string path = get_attr("path", node);

    if (path.empty()){
        ERR("cannot find \"" + path + "\" floor attribute");
        return false;
    }

    
    world_top.scale(get_attr("scale", node));
    return world_top.load(path);
}

bool Game::is_everything_loaded(void) const{
    return (is_floor_loaded && is_top_loaded && is_collisions_loaded && is_Astar_loaded);
}

bool Game::blit_floor(void){
    GPU_Image *image = world_floor.image();
    if (!image) return false;

    image->base_w = image->w * _zoom * world_floor.scale();
    image->base_h = image->h * _zoom * world_floor.scale();
    GPU_Rect src = {0, 0, float(image->base_w), float(image->base_h)};
    GPU_Blit(image, &src, _target, -_x + (image->w * _zoom) / 2, -_y + (image->h * _zoom) / 2);
    return true;
}

bool Game::blit_top(void){
    GPU_Image *image = world_top.image();
    if (!image) return false;

    image->base_w = image->w * _zoom * world_top.scale();
    image->base_h = image->h * _zoom * world_top.scale();
    GPU_Rect src = {0, 0, float(image->base_w), float(image->base_h)};
    GPU_Blit(image, &src, _target, -_x + (image->w * _zoom) / 2, -_y + (image->h * _zoom) / 2);
    return true;
}

void Game::update_widgets(void){
    for (auto &w : widgets){
        w->OnTick(delta_tick);
    }

    if (events.isButtonPush(event::Mouse_button_left)){
        for (auto &w : widgets){
            
            if (w->is_button()){
                if (w->is_mouse_hover()){
                    load_menu(w->get());
                    break;
                }
            } else if (w->is_switch()){
                if (w->is_mouse_hover()){
                    w->active();
                }
            }
        }
    }
}

void Game::blit_widgets(void){
    for (auto &w : widgets){
        w->OnDraw(_target);

        if (render_widget_border){
            const int width = w->w() / 2;
            const int height = w->h() / 2;

            GPU_Rectangle(_target, w->x() - width+2, w->y() - height+2, w->x() + width-2, w->y() + height-2, {255, 0, 0, 255});
        }

        if (render_hovered_widget_border && w->is_mouse_hover()){
            const int width = w->w() / 2;
            const int height = w->h() / 2;

            GPU_Rectangle(_target, w->x() - width+2, w->y() - height+2, w->x() + width-2, w->y() + height-2, {0, 255, 0, 255});
        }
    }
}

void Game::blit_widgets_HUD(void){
    for (auto &w : widgets){
        w->OnHUDdraw(_target);
    }
}

void Game::reset_keys(void){
    pause_key = SDL_SCANCODE_ESCAPE;
    debug_key = SDL_SCANCODE_F11;
    cam_up_key = SDL_SCANCODE_UP;
    cam_down_key = SDL_SCANCODE_DOWN;
    cam_left_key = SDL_SCANCODE_LEFT;
    cam_right_key = SDL_SCANCODE_RIGHT;
    cam_zoom_in_key = SDL_SCANCODE_O;
    cam_zoom_out_key = SDL_SCANCODE_L;
}

bool Game::load_menu(std::string path){
    if (path.empty()) return false;
    widgets.clear();
    const int start = SDL_GetTicks();
    if (path[1] != ':') path = RES + path;
    LOAD_LOG(path);

    XMLDocument doc;
    if (XMLDocument_load(&doc, path.c_str())){
        for (int c=0; c<doc.root->children.size; c++){
            XMLNode *child = XMLNode_child(doc.root, c);

            if (is_equal(child->tag, "text")){
                load_text_widget(child);

            } else if (is_equal(child->tag, "rect") || is_equal(child->tag, "rectangle")){
                load_rect_widget(child);
            
            } else if (is_equal(child->tag, "border")){
                load_border_widget(child);
            
            } else if (is_equal(child->tag, "textButton")){
                load_textButton_widget(child);
            
            } else if (is_equal(child->tag, "boolTextButton")){
                load_boolTextButton_widget(child);
            
            } else if (is_equal(child->tag, "clear/") || is_equal(child->tag, "clear")){
                is_menu_opened = false;
                XMLDocument_free(&doc);
                return true;
            
            } else if (is_equal(child->tag, "quit/") || is_equal(child->tag, "quit")){
                is_menu_opened = false;
                launched = false;
                XMLDocument_free(&doc);
                return true;
            
            }  else if (is_equal(child->tag, "menu/") || is_equal(child->tag, "menu")){
                load_save("data\\worlds\\menu_map.xml");
                XMLDocument_free(&doc);
                return true;

            } else {
                WARN("cannot reconize \"" + std::string(child->tag) + "\" widget tag");
            }
        }
    } else {
        return false;
    }

    curr_menu = path;
    is_menu_opened = true;

    LOG("widget xml file loaded with success, duration : " + std::to_string(SDL_GetTicks() - start));
    XMLDocument_free(&doc);
    return true;
}

bool Game::load_text_widget(XMLNode *node){
    LOG("load text widget");
    std::shared_ptr<Text> text = std::make_shared<Text>();
    text->set_events(&events);
    text->set_ttf(&fonts);
    text->set_window_size(&window_w, &window_h);

    if (text->load(node)){
        widgets.push_back(text);
    } else {
        return false;
    }
    return true;
}

bool Game::load_rect_widget(XMLNode *node){
    LOG("load rect widget");
    std::shared_ptr<Rect> rect = std::make_shared<Rect>();
    rect->set_events(&events);
    rect->set_window_size(&window_w, &window_h);

    if (rect->load(node)){
        widgets.push_back(rect);
    } else {
        return false;
    }
    return true;
}

bool Game::load_border_widget(XMLNode *node){
    LOG("load broder widget");
    std::shared_ptr<Border> border = std::make_shared<Border>();
    border->set_events(&events);
    border->set_window_size(&window_w, &window_h);

    if (border->load(node)){
        widgets.push_back(border);
    } else {
        return false;
    }
    return true;
}

bool Game::load_textButton_widget(XMLNode *node){
    LOG("load text button widget");
    std::shared_ptr<TextButton> btn = std::make_shared<TextButton>();
    btn->set_events(&events);
    btn->set_ttf(&fonts);
    btn->set_window_size(&window_w, &window_h);

    if (btn->load(node)){
        widgets.push_back(btn);
    } else {
        return false;
    }
    return true;
}

bool Game::load_boolTextButton_widget(XMLNode *node){
    LOG("load bool text button widget");
    std::shared_ptr<BoolTextButton> btn = std::make_shared<BoolTextButton>();
    btn->set_events(&events);
    btn->set_ttf(&fonts);
    btn->set_window_size(&window_w, &window_h);

    if (btn->load(node)){
        btn->set_value(get_value(btn->get_value_name()));
        widgets.push_back(btn);
    } else {
        return false;
    }
    return true;
}

inline static std::string get_value_category(std::string full_name){
    std::size_t separator = full_name.find(".");
    
    if (separator == std::string::npos)
        return "";

    std::string category;
    for (int c=0; c<separator; c++){
        category += full_name[c];
    }

    return category;
}

bool *Game::get_value(std::string value_name){
    std::string category = get_value_category(value_name);

    if (category == "widget")
        return get_value_widget(value_name);
    
    if (category == "shadowCaster")
        return get_value_shadowCaster(value_name);
    
    if (category == "camera")
        return get_value_camera(value_name);

    WARN("cannot reconize \"" + category + "\" value category in \"" + value_name + "\" value name");
    return nullptr;
}

bool *Game::get_value_widget(std::string value){
    if (value == "widget.render_border")
        return &render_widget_border;

    if (value == "widget.render_hovered_border")
        return &render_hovered_widget_border;
    

    WARN("cannot reconize \"" + value + "\" in widget category");
    return nullptr;
}

bool *Game::get_value_shadowCaster(std::string value){

    if (value == "shadowCaster.render_borders")
        return &render_shadowCaster_borders;
    
    if (value == "shadowCaster.render_light_poly")
        return &render_shadowCaster_polygones;

    WARN("cannot reconize \"" + value + "\" in shadowCaster category");
    return nullptr;
}

bool *Game::get_value_camera(std::string value){
    if (value == "camera.free")
        return &free_camera;
    
    WARN("cannot reconize \"" + value + "\" in camera category");
    return nullptr;
}

void Game::update_cam_events(void){
    if (free_camera){
        if (events.IsKeyDown(cam_up_key))
            ty -= 0.1 * delta_tick;

        if (events.IsKeyDown(cam_down_key))
            ty += 0.1 * delta_tick;
        
        if (events.IsKeyDown(cam_left_key))
            tx -= 0.1 * delta_tick;
        
        if (events.IsKeyDown(cam_right_key))
            tx += 0.1 * delta_tick;
        
        if (events.IsKeyDown(cam_zoom_in_key))
            _zoom -= 0.01 * delta_tick;
        
        if (events.IsKeyDown(cam_zoom_out_key))
            _zoom += 0.01 * delta_tick;
    }
}

void Game::update_entitys(void){
    for (auto &e : entitys){
        e->OnTick(delta_tick);
    }
}

void Game::blit_entitys(void){
    for (auto &e : entitys){
        e->OnDraw(_target, _x, _y, _zoom);
    }
}

void Game::blit_lightSources_poly(void){
    if (render_shadowCaster_polygones){
        for (auto &l : lightSources){
            blit_lightSource_poly(l);
        }
    }
}

void Game::blit_lightSource_poly(std::shared_ptr<light::LightSource> light){
    for (auto &p : light->get_vibility_poly()){
        GPU_Line(_target, light->x() * _zoom - _x, light->y() * _zoom - _y, p.x, p.y, {255, 0, 0, 255});
        GPU_Rectangle(_target, (p.x - 5) * _zoom - _x, (p.y - 5) * _zoom - _y, (p.x + 5) * _zoom - _x, (p.y + 5) * _zoom - _y, {255, 0, 0, 255});
    }
    GPU_Circle(_target, light->x() * _zoom - _x, light->y() * _zoom - _y, 10 * _zoom, {255, 0, 0, 255});
}
