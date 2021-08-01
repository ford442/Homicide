#include "Game.hpp"
#include "dir.hpp"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <filesystem>

#include "csv.hpp"

#define _ERRS
#define _WARNS
#define _LOGS

#ifdef _ERRS
    #define ERR(msg) std::cerr << "ERROR :: " << __func__ << " : " << msg << std::endl;
#else
    #define ERR(msg)
#endif

#ifdef _WARNS
    #define WARN(msg) std::cerr << "WARNING :: " << __func__ << " : " << msg << std::endl;
#else
    #define WARN(msg)
#endif

#ifdef _LOGS
    #define LOG(msg) std::cout << "INFO :: " << __func__ << " : " << msg << std::endl;
#else
    #define LOG(msg)
#endif

using G = Game;

int InitSDL_SUB_Libs(void *ptr);

G::Game(void){
    launched = false;
    _window = nullptr;
    _event_handler = nullptr;
    _world = nullptr;
    _lightImageList = nullptr;
    _player = nullptr;
    _camera = nullptr;
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

    Init_logs();
    std::cout << "INFO :: Init librarys" << std::endl;
    Init_Window();
    bool error = InitSDL_SUB_Libs(this);
    std::cout << "INFO :: SDL2 libs initalized" << std::endl;

    _camera = std::make_shared<Camera>();
    _camera->set_delay(350);
    _camera->setMovementType(Camera::Camera_FadeIn);
    _event_handler = std::make_shared<event::Handler>();
    _animations = std::make_shared<sprite::Animations>();
    _entityList = std::make_shared<entity::EntityList>();
    _lightImageList = std::make_shared<light::LightImageList>(&_pixel_size, &_x, &_y);
    debug_mod = false;

    std::string vert_filename(RES + "shaders\\blur\\vert.glsl");
    std::string frag_filename(RES + "shaders\\blur\\frag.glsl");

    blur_vert = GPU_LoadShader(GPU_VERTEX_SHADER, vert_filename.c_str());

    if (!blur_vert){
        std::cerr << "ERROR :: cannot load \"" << vert_filename << "\"" << std::endl;
        return false;
    }

    blur_frag = GPU_LoadShader(GPU_FRAGMENT_SHADER, frag_filename.c_str());

    if (!blur_frag){
        std::cerr << "ERROR :: cannot load \"" << frag_filename << "\"" << std::endl;
        return false;
    }

    blur_shader = GPU_LinkShaders(blur_vert, blur_frag);

    if (!blur_shader){
        std::cerr << "ERROR :: cannot link blur shader" << std::endl;
        return false;
    }

    blur = GPU_LoadShaderBlock(blur_shader, "Position", "TexCoord", "Color", "u_projView");

    blur_resolution = GPU_GetUniformLocation(blur_shader, "resolution");
    if (blur_resolution == -1){
        std::cerr << "ERROR :: cannot found the uniform value \"resolution\" in : \"" << frag_filename << "\"" << std::endl;
        return false;
    }

    blur_radius = GPU_GetUniformLocation(blur_shader, "radius");
    if (blur_radius == -1){
        std::cerr << "ERROR :: cannot found the uniform value \" radius \" in : \"" << frag_filename << "\"" << std::endl;
        return false;
    }

    _x = 0;
    _y = 0;

    set_max_fps(60);
    launched = true;

    std::cout << std::endl << "INFO :: game launched success : " << (launched == true ? "true" : "false") << " ticks : " << SDL_GetTicks() << " milisecond" << std::endl << std::endl;

    load_settings_file("data\\options.csv");

    _pixel_size = window_w / 450;
    if (!load_world("worlds\\menu_map.xml")) return false;

    if (!_world->get_light()->set_shader(blur_shader)) return false;

    _player = _world->get_player();
    _player->set_location(50, 50);
    _entityList->push_back(_player);

    std::cout << "INFO :: game initialization ended" << std::endl;
    return error;
}

std::string getCurrentDateTime(std::string s){
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];

    tstruct = *localtime(&now);
    if(s=="now")
        strftime(buf, sizeof(buf), "%Y-%m-%d-%X", &tstruct);
    else if(s=="date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

    return std::string(buf);
}

std::string log_dir(void){
    return LOGS + getCurrentDateTime("date");
}

bool initLogDir(void){
    std::cout << log_dir() << std::endl;
    return std::filesystem::create_directories(log_dir());
}

bool G::Init_logs(void){
    GPU_SetDebugLevel(GPU_DEBUG_LEVEL_3);

    if (!std::filesystem::exists(log_dir())){
        if (!initLogDir()){
            std::cerr << "ERROR :: cannot create the log folder" << std::endl;
            return false;
        }
    }

    std::string log_path = log_dir() + "\\log_" + getCurrentDateTime("date") + ".log";
    freopen(log_path.c_str(), "w", stdout);

    std::cout << "LOG file :: init at " << getCurrentDateTime("now") << std::endl;
 
    std::string err_path = log_dir() + "\\err_" + getCurrentDateTime("date") + ".log";
    std::cout << "open err file : \"" << err_path << "\"" << std::endl;
    freopen(err_path.c_str(), "w", stderr);

    std::cerr << "ERROR file :: init at " << getCurrentDateTime("now") << std::endl;
    
    return true;
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
                        _pixel_size = e.window.data1 / 450;
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

        _event_handler->event(&e);
    }
}

void G::draw(void){
    GPU_Clear(_target);

    
    GPU_ActivateShaderProgram(blur_shader, &blur);

    GPU_SetUniformf(blur_resolution, 5000.0f);
    GPU_SetUniformf(blur_radius, 10.0f);
    _world->draw_light_poly(_target);
    
    GPU_ActivateShaderProgram(0, nullptr);

    // _entityList->OnLightDraw(_world->get_enlightened_target());

    _world->draw(_target);
    _world->blit(_target);
    
    for (auto p : projectiles){
        p->draw(_target, _x, _y, _pixel_size);
    }

    _world->draw_top(_target, debug_mod);

    GPU_Flip(_target);
}

void Game::shoot(int x, int y, int dir){
    std::shared_ptr<Projectile> p = std::make_shared<Projectile>();

    p->set_type(projectile_types.front());
    p->set_x(x);
    p->set_y(y);
    p->set_angle(dir);
    projectiles.push_back(p);
}

void G::update(void){

    if (_event_handler->IsKeyDown(SDL_SCANCODE_O))
        _pixel_size += 0.1;

    if (_event_handler->IsKeyDown(SDL_SCANCODE_L))
        _pixel_size -= 0.1;
    
    if (_event_handler->IsKeyPush(SDL_SCANCODE_F3))
        debug_mod = !debug_mod;
    
    if (_event_handler->isButtonDown(event::Mouse_button_left)){
        shoot(_player->get_x() / _pixel_size, _player->get_y() / _pixel_size, _player->get_facing());
    }

    for (auto p : projectiles){
        if (!p->OnTick(delta_tick, _world->get_collisions())){
            projectiles.remove(p);
        }
    }
    
    _world->OnTick(delta_tick);
    _entityList->OnTick(delta_tick);
    _entityList->updateMovements(delta_tick);
    _entityList->OnMouseMovement(_event_handler->mouse_x(), _event_handler->mouse_y());
    _entityList->updateAnimations(delta_tick);
    

    _camera->OnTick(delta_tick);
    _camera->go_to(_player->get_x(), _player->get_y());
    float x, y;
    _camera->get_pos(&x, &y);
    _x = x - window_w / 2 + (_event_handler->mouse_x() - (window_w / 2)) / 10;
    _y = y - window_h / 2 + (_event_handler->mouse_y() - (window_h / 2)) / 10;
    
    _event_handler->update();
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

float G::get_pixel_size(void) const{
    return _pixel_size;
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

bool G::load_world(std::string world_path){
    int tick = SDL_GetTicks();
    _world = std::make_shared<world::World>(world_path, _animations, &_pixel_size, &_x, &_y, _lightImageList, _player, _world, _event_handler, _entityList);
    std::cout << std::endl << "INFO :: world loading finish, loading time : " << SDL_GetTicks() - tick << " milisecond" << std::endl << std::endl;
    return true;
}

void G::quit(void){
    std::cout << std::endl;
    int tick = SDL_GetTicks();
    std::cout << "INFO :: releasing memory from a Game instance" << std::endl;
    if (_window) SDL_DestroyWindow(_window);
    if (_target) GPU_FreeTarget(_target);

    _event_handler = nullptr;
    _world = nullptr;
    _animations = nullptr;
    _entityList = nullptr;
    _lightImageList = nullptr;
    _player = nullptr;
    _camera = nullptr;

    std::cout << "INFO :: quit SDL2 libs" << std::endl;

    GPU_FreeShader(blur_shader);

    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    tick = SDL_GetTicks() - tick;
    projectile_types.clear();

    GPU_Quit();

    std::cout << std::endl << "INFO :: Game memory release finish, release execution tick : " << tick << " milisecond" << std::endl << std::endl;
}

int InitSDL_SUB_Libs(void *ptr){
    Game *g = (Game*)ptr;
    
    std::cout << "bonjour" << std::endl;
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

        auto projectile = std::make_shared<Projectile_type>();

        if (projectile->load(dir_path + f, _lightImageList)){
            projectile_types.push_back(projectile);
        }
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

        auto light = std::make_shared<light::LightImage>(&_pixel_size, &_x, &_y);

        if (light->load_csv(dir_path + f)){
            _lightImageList->push(light);
        }
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

        auto spriteSheet = std::make_shared<sprite::SpriteSheet>();

        if (spriteSheet->load_csv(dir_path + f)){
            _animations->push(spriteSheet);
        }
    }
    return true;
}