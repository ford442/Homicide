#ifndef __PROJECTILE__TYPE__HPP__
    #define __PROJECTILE__TYPE__HPP__

    #include <SDL2/SDL_gpu.h>
    #include <iostream>

    #include "lights/LightImage.hpp"
    #include "lights/LightImageList.hpp"

    class Projectile_type{
        public:
            Projectile_type();
            ~Projectile_type();

            GPU_Image *get_image(void) const;
            std::shared_ptr<light::LightImage> get_light_image(void);

            bool load(std::string path, std::shared_ptr<light::LightImageList> lights);

        private:
            GPU_Image *image;
            std::shared_ptr<light::LightImage> light;

            int damages;
            float speed;
            std::string name;
    };

#endif