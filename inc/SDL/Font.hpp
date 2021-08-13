#ifndef __FONT__HPP__
    #define __FONT__HPP__

    #include <iostream>
    #include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_gpu.h>

    class Font{
        public:
            Font();
            ~Font();

            bool load(std::string path);
            bool reload(void);
            bool set_size(int size);

            TTF_Font* get_font(void);

            std::string get_name(void);

            SDL_Surface *solid_surface(std::string text, SDL_Color fg);
            SDL_Surface *shaded_surface(std::string text, SDL_Color fg, SDL_Color bg);
            SDL_Surface *blended_surface(std::string text, SDL_Color fg);

            GPU_Image *solid_image(std::string text, SDL_Color fg);
            GPU_Image *shaded_image(std::string text, SDL_Color fg, SDL_Color bg);
            GPU_Image *blended_image(std::string text, SDL_Color fg);

        private:

            void destroy_font(void);

            TTF_Font* font;

            std::string font_path;
            std::string name;
            int size;
    };

#endif