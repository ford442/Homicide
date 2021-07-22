#include "SDL/Window.hpp"

using W = SDL::Window;

#define _LOG
#define _ERR
#define _WARN

#ifdef _LOG
    #define LOG(msg) std::cout << "INFO :: in " << __FILE__ << " from " << __func__ << " at line " << __LINE__ << ", msg : " << msg << std::endl;
#else
    #define LOG(msg)
#endif

#ifdef _ERR
    #define ERR(msg) std::cerr << "ERROR :: in " << __FILE__ << " from " << __func__ << " at line  " << __LINE__ << ", msg : " << msg << std::endl;
#else
    #define ERR(msg)
#endif

#ifdef _WARN
    #define WARN(msg) std::cerr << "WARNING :: in " << __FILE__ << " from " << __func__ << " at line " << __LINE__ << ", msg : " << msg << std::endl;
#else
    #define WARN(msg)
#endif


#define check(value, msg, rtn) \
    if(!value){ \
        ERR(msg); \
        return rtn; \
    }

W::Window(){
    LOG("new window instance");
    _window = nullptr;
}

W::~Window(){
    LOG("releasing window instance");
    destroy();
}

void W::w(const int width){
    int w = width;
    set_size(&w, nullptr);
}

void W::h(const int height){
    int h = height;
    set_size(nullptr, &h);
}

void W::size(const int w, const int h){
    int width=this->w(), height=this->h();
    set_size(&width, &height);
}

int W::w(void) const{
    int w;
    size(&w, nullptr);
    return w;
}

int W::h(void) const{
    int h;
    size(nullptr, &h);
    return h;
}

void W::size(int *w, int *h) const{
    check(_window, "window node initialized",);
    SDL_GetWindowSize(_window, w, h);
}

void W::set_size(int *w, int *h){
    check(_window, "window not initialized",);

    int width = this->w(), height = this->h();
    if (w) width = *w;
    if (h) height = *h;

    SDL_SetWindowSize(_window, width, height);
}

SDL_Window *W::window(void) const{
    return _window;
}

void W::x(const int x){
    int _x = x;
    set_pos(&_x, nullptr);
}

void W::y(const int y){
    int _y = y;
    set_pos(&_y, nullptr);
}

void W::pos(const int x, const int y){
    int _x=this->x(), _y=this->y();
    set_pos(&_x, &_y);
}

int W::x(void){
    int _x;
    pos(&_x, nullptr);
    return _x;
}

int W::y(void){
    int _y;
    pos(nullptr, &_y);
    return _y;
}

void W::pos(int *x, int *y){
    check(_window, "window not initialized",);
    SDL_GetWindowSize(_window, x, y);
}

void W::set_pos(int *x, int *y){
    check(_window, "window not initialized",);
    int _x = this->x(), _y = this->y();

    if (x) _x = *x;
    if (y) _y = *y;

    SDL_SetWindowPosition(_window, _x, _y);
}

bool W::create(Uint32 flags){
    if (_window){
        ERR("window early initialized");
        return false;
    }

    _window = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, flags);
    return true;
}

void W::is_border(bool bordered){
    check(_window, "window not initialized",);
    SDL_SetWindowBordered(_window, (SDL_bool)bordered);
}

void W::minimum_size(int *w, int *h){
    check(_window, "window not initialized",);
    SDL_GetWindowMinimumSize(_window, w, h);
}

void W::maximum_size(int *w, int *h){
    check(_window, "window not initialized",);
    SDL_GetWindowMaximumSize(_window, w, h);
}

void W::set_window_fullscreen(Uint32 flag){
    check(_window, "window not initialized",);
    SDL_SetWindowFullscreen(_window, flag);
}

void W::title(std::string title){
    check(_window, "window not initialized",);
    SDL_SetWindowTitle(_window, title.c_str());
}

std::string W::title(void) const{
    check(_window, "window not initialized", "");
    return SDL_GetWindowTitle(_window);
}

void W::destroy(void){
    if (_window){
        LOG("destroying SDL_Window instance");
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}
