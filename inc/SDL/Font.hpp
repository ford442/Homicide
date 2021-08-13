#ifndef __FONT__HPP__
    #define __FONT__HPP__

    #include <iostream>
    #include <SDL2/SDL_ttf.h>

    class Font{
        public:
            Font();
            ~Font();

            bool load(std::string path);
            bool reload(void);

            TTF_Font* get_font(void);

        private:

            void destroy_font(void);

            TTF_Font* font;

            std::string font_path;
            int size;
    };

#endif