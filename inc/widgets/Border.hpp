#ifndef __BORDER_HPP__
    #define __BORDER_HPP__

    #include <iostream>
    #include <SDL2/SDL_gpu.h>
    #include "xml.hpp"
    #include "widgets/Rect.hpp"

    class Border : public Rect{
        public:
            Border();
            ~Border();
            
            void OnDraw(GPU_Target *t) override;
            bool load(XMLNode *node) override;

            const float border_width(void) const;
            void border_width(const float width);

        private:
            
            float width;
    };

#endif