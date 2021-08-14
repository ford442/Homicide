#ifndef __TEXTBUTTON_HPP__
    #define __TEXTBUTTON_HPP__

    #include <iostream>
    #include <functional>
    #include "widgets/Rect.hpp"
    #include "widgets/Text.hpp"
    #include "widgets/Border.hpp"

    class Button : public Rect, public Border{
        public:
            Button();
            ~Button();
            
            const std::string get_path(void) const;
            void set_path(const std::string path);

        private:

            std::string path;

    };

#endif