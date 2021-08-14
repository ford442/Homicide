#ifndef __TEXT__HPP__
    #define __TEXT__HPP__

    #include <iostream>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_gpu.h>
    #include "widgets/Widget.hpp"
    #include "SDL/Font.hpp"
    #include "SDL/TTF.hpp"

    class Text : public Widget{
        public:
            Text();
            ~Text();
            
            void set_text(std::string text);
            void set_forground_color(SDL_Color color);
            void set_background_color(SDL_Color color);
            void set_font(std::shared_ptr<Font> font);
            void set_render_type(Font::Render_type type);
            void set_ttf(TTF* ttf);

            bool update(void);
            virtual void OnDraw(GPU_Target *t) override;
            virtual void OnHUDdraw(GPU_Target *t) override;
            virtual bool load(XMLNode *node) override;

            GPU_Image *get_image(void) const;
        
        private:

            std::string text;
            GPU_Image *image;
            SDL_Color fg, bg;
            std::shared_ptr<Font> font;
            TTF* ttf;
            Font::Render_type render_type;
    };

#endif