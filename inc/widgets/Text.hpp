#ifndef __TEXT__HPP__
    #define __TEXT__HPP__

    #include <iostream>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_gpu.h>
    #include "widgets/Widget.hpp"
    #include "SDL/Font.hpp"

    class Text : public Widget{
        public:
            Text();
            ~Text();
            
            bool set_text(std::string text);
            bool set_forground_color(SDL_Color color);
            bool set_background_color(SDL_Color color);
            bool set_font(Font *font);
            bool set_render_type(Font::Render_type type);

            bool update(void);
            virtual void OnDraw(GPU_Target *t) override;

            GPU_Image *get_image(void) const;
        
        private:

            std::string text;
            GPU_Image *image;
            SDL_Color fg, bg;
            Font* font;
            Font::Render_type render_type;
    };

#endif