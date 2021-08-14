#ifndef __BUTTON_HPP__
    #define __BUTTON_HPP__

    #include <iostream>
    #include <functional>
    #include "widgets/Widget.hpp"

    class Button : public Widget{
        public:
            Button();
            ~Button();
            
            const std::string get_path(void) const;
            void set_path(const std::string path);

            bool is_mouse_hover(void);

            bool load(XMLNode *node) override;

        private:

            std::string path;

    };

#endif