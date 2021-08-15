#ifndef __BOOLTEXTBUTTON_HPP__
    #define __BOOLTEXTBUTTON_HPP__

    #include <iostream>
    #include "widgets/Text.hpp"

    class BoolTextButton : public Text{
        public:
            BoolTextButton();
            ~BoolTextButton();

            void set_value(bool *value);
            
            void OnDraw(GPU_Target *t) override;
            bool load(XMLNode *node) override;
            bool is_switch(void) const override;
            void active(void) override;
            

            std::string get_value_name(void) const;
        private:
            bool *value;
            std::string value_name;
    };

#endif