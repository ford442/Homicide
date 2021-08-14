#ifndef __TTF__HPP__
    #define __TTF__HPP__

    #include <iostream>
    #include <list>
    #include <memory>
    #include <SDL2/SDL_ttf.h>
    #include "SDL/Font.hpp"

    class TTF{
        public:

            TTF();
            ~TTF();
            
            bool init(void);
            bool is_init(void) const;

            bool load_font(std::string path);
            bool load_font_dir(std::string dir);

            std::shared_ptr<Font> get(std::string name);

        private:

            std::list<std::shared_ptr<Font>> fonts;
            bool is_ttf_init;
    };

#endif