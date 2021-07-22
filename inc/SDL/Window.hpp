#ifndef __WINDOW__HPP__
    #define __WINDOW__HPP__

    #include <iostream>
    #include <SDL2/SDL.h>

    namespace SDL{
        class Window{
            public:
                Window();
                ~Window();

                bool create(Uint32 flags);

                void w(const int width);
                void h(const int height);
                void size(const int w, const int h);

                int w(void) const;
                int h(void) const;
                void size(int *w, int *h) const;

                void x(const int x);
                void y(const int y);
                void pos(const int x, const int y);

                int x(void);
                int y(void);
                void pos(int *x, int *y);

                void is_border(bool bordered);

                void minimum_size(int *w, int *h);
                void maximum_size(int *w, int *h);

                void set_window_fullscreen(Uint32 flag);

                void title(std::string title);
                std::string title(void) const;
                
                SDL_Window *window(void) const;

                void destroy(void);

            private:
                SDL_Window *_window;

                void set_size(int *w, int *h);
                void set_pos(int *x, int *y);
        };
    }

#endif