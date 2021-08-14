#ifndef __RECT_HPP__
    #define __RECT_HPP__

    #include <iostream>
    #include "widgets/Widget.hpp"

    class Rect : public Widget{
        public:
            Rect();
            ~Rect();

            void OnDraw(GPU_Target *t) override;
            bool load(XMLNode *node) override;

            void set_color(SDL_Color color);
            void set_color(std::string hexa);
            void set_color(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);
            SDL_Color get_color(void) const;
        
        private:
            SDL_Color fg;
    };

#endif