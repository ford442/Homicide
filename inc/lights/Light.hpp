#ifndef __LIGHT__HPP__
    #define __LIGHT__HPP__

    #include <iostream>
    #include <memory>
    #include <list>
    #include <SDL2/SDL_gpu.h>
    #include <SDL2/SDL.h>

    #include "ShadowCaster.hpp"
    #include "xml.hpp"

    class Light{
        public:
            Light();
            ~Light();

            /**
             * @brief load the light from a xml node
             * @param node the xml node
             * @return true if loaded, false on error
             */
            bool load(XMLNode *node);

            /**
             * @brief load the light from a csv file
             * @param path the path to the .csv file can be relativ or complete
             * @return true if loaded, false on error
             */
            bool load(std::string path);

            GPU_Image *get_image(void) const;
            GPU_Target *get_target(void) const;

        private:

            void destroy_shadow(void);
            bool uupdate_size(int &w, int &h);

            std::shared_ptr<ShadowCaster> shadow_caster;

            GPU_Target *shadow_target;
            GPU_Image *shadow_image;

            SDL_Color clear_color;

            // std::list<> sources;
    };


#endif