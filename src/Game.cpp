#include "Game.hpp"
#include "dir.hpp"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <filesystem>

#include "csv.hpp"
#include "main.hpp"

using G = Game;

int InitSDL_SUB_Libs(void *ptr);

G::Game(void){
    launched = false;
    _window = nullptr;
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
    std::cout << "Init_TTF()" << std::endl;
    if (TTF_Init()){
        std::cerr << "ERROR :: TTF_Init : " << TTF_GetError << std::endl;
        return false;
    }
    return true;
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

    load_settings_file("data\\options.csv");

    _zoom = window_w / 450;
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
    blit_top();

    // blur.unactive();

    GPU_Flip(_target);
}

void Game::shoot(int x, int y, int dir){
    // std::shared_ptr<Projectile> p = std::make_shared<Projectile>();

    // p->set_type(projectile_types.front());
    // p->set_x(x);
    // p->set_y(y);
    // p->set_angle(dir);
    // projectiles.push_back(p);
}

void G::update(void){

    if (events.IsKeyDown(SDL_SCANCODE_O))
        _zoom += 0.1;

    if (events.IsKeyDown(SDL_SCANCODE_L))
        _zoom -= 0.1;
    
    if (events.IsKeyPush(SDL_SCANCODE_F3))
        debug_mod = !debug_mod;
    
    // if (events.isButtonDown(event::Mouse_button_left)){
    //     shoot(_player->get_x() / _zoom, _player->get_y() / _zoom, _player->get_facing());
    // }

    // for (auto p : projectiles){
    //     int normal;
    //     bool is_wall;

    //     if (!p->OnTick(delta_tick, _world->get_collisions(), &normal, &is_wall)){
    //         projectiles.remove(p);
    //     }
    // }
    
    // _world->OnTick(delta_tick);
    // _entityList->OnTick(delta_tick);
    // _entityList->updateMovements(delta_tick);
    // _entityList->OnMouseMovement(events.mouse_x(), events.mouse_y());
    // _entityList->updateAnimations(delta_tick);
    

    _camera.OnTick(delta_tick);
    _camera.go_to(events.mouse_x(), events.mouse_y());

    float x, y;
    _camera.get_pos(&x, &y);
    _x = x - window_w / 2 + (events.mouse_x() - (window_w / 2)) / 10;
    _y = y - window_h / 2 + (events.mouse_y() - (window_h / 2)) / 10;
    
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

    if (!load_lights(doc.search("lights"))) return false;
    if (!load_projectiles(doc.search("projectiles"))) return false;
    if (!load_animations(doc.search("animations"))) return false;
    if (!load_weapons(doc.search("weapons"))) return false;

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
    window_h = h;
}

void Game::set_window_height(int h){
    if (!_window){
        ERR("cannot set the title of a null window");
        return;
    }
    int w;
    SDL_GetWindowSize(_window, &w, nullptr);
    GPU_SetWindowResolution(w, h);
    window_w = w;
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

bool Game::load_animations(std::string dir_path){
    if (dir_path[1] != ':') dir_path = RES + dir_path;

    std::vector<std::string> dir_content = dir::content(dir_path);

    if (dir_content.empty()){
        ERR("directory \"" + std::string(dir_path) + "\" not found or empty");
        return false;
    }

    for (auto &f : dir_content){
        if (f == ".." || f == ".") continue;

        // auto spriteSheet = std::make_shared<sprite::SpriteSheet>();

        // if (spriteSheet->load_csv(dir_path + f)){
        //     _animations->push(spriteSheet);
        // }
    }
    return true;
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

    GPU_Clear(_target);

    XMLDocument doc;
    if (XMLDocument_load(&doc, path.c_str())){
        for (int c=0; c<doc.root->children.size; c++){
            XMLNode *child = XMLNode_child(doc.root, c);

            if (is_equal(child->tag, "floor")){
                is_floor_loaded = load_world_floor(child);
            
            } else if (is_equal(child->tag, "top")){
                is_top_loaded = load_world_top(child);
            
            } else if (is_equal(child->tag, "ShadowCaster")){
                if (!shadow_layer.load(child)){
                    err = true;
                    break;
                }
            
            } else if (is_equal(child->tag, "collisions")){
            //     if (!collisions.load(child)){
            //         err = true;
            //         break;
            //     }
            
            // } else if (is_equal(child->tag, "Astar")){
            //     if (!Astar.load(child)){
            //         err = true;
            //         break;
            //     }
            }
            
            GPU_Clear(_target);
            GPU_SectorFilled(_target, window_w / 2, window_h / 2, window_w / 11, window_w / 10, 0, float(c) / float(doc.root->children.size) * 360, {255, 255, 255, 255});
            GPU_Flip(_target);
        }
    } else {
        return false;
    }

    XMLDocument_free(&doc);
    return !err;
}

bool Game::load_world_floor(XMLNode *node){
    std::string path = get_attr("path", node);

    if (path.empty()){
        ERR("cannot find \"" + path + "\" floor attribute");
        return false;
    }

    return world_floor.load(path);
}

bool Game::load_world_top(XMLNode *node){
    std::string path = get_attr("path", node);

    if (path.empty()){
        ERR("cannot find \"" + path + "\" floor attribute");
        return false;
    }

    return world_top.load(path);
}

bool Game::is_everything_loaded(void) const{
    return (is_floor_loaded && is_top_loaded && is_collisions_loaded && is_Astar_loaded);
}

bool Game::blit_floor(void){
    GPU_Image *image = world_floor.image();
    if (!image) return false;

    image->base_w = image->w * _zoom;
    image->base_h = image->h * _zoom;
    GPU_Rect src = {0, 0, float(image->base_w), float(image->base_h)};
    GPU_Blit(image, &src, _target, -_x + (image->w * _zoom) / 2, -_y + (image->h * _zoom) / 2);
    return true;
}

bool Game::blit_top(void){
    GPU_Image *image = world_top.image();
    if (!image) return false;

    image->base_w = image->w * _zoom;
    image->base_h = image->h * _zoom;
    GPU_Rect src = {0, 0, float(image->base_w), float(image->base_h)};
    GPU_Blit(image, &src, _target, -_x + (image->w * _zoom) / 2, -_y + (image->h * _zoom) / 2);
    return true;
}