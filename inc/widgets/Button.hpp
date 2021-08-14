#ifndef __BUTTON_HPP__
    #define __BUTTON_HPP__

    #include <iostream>
    #include <functional>
    #include "xml.hpp"

    class Button{
        public:
            Button();
            ~Button();
            
            const std::string get_path(void) const;
            void set_path(const std::string path);

            virtual bool load(XMLNode *node);

        private:

            std::string path;

    };

#endif