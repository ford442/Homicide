#ifndef __TEXTBUTTON_HPP__
    #define __TEXTBUTTON_HPP__

    #include <iostream>
    #include "widgets/Button.hpp"
    #include "widgets/Text.hpp"

    class TextButton : public Button, public Text{
        public:
            TextButton();
            ~TextButton();

            void OnDraw(GPU_Target *target) override;
            bool load(XMLNode *node) override;
        
        private:

            void x(const float x);
            void y(const float y);

            const float x(void) const;
            const float y(void) const;
    };

#endif