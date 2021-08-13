#ifndef __TTF__HPP__
    #define __TTF__HPP__

    #include <iostream>
    #include <list>
    #include <SDL2/SDL_ttf.h>

    class TTF{
        public:
            struct Font{
                TTF_Font* font;
                std::string name;
                int size;
            };

            TTF();
            ~TTF();
            
            bool init(void);
            bool load_font(std::string path);

        private:

            std::list<Font> fonts;
    };

#endif