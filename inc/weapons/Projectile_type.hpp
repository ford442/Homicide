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

            int get_damages(void) const;
            float get_max_speed(void) const;
            float get_min_speed(void) const;
            float get_rand_speed(void) const;
            
            float get_caliber(void) const;

        private:

            bool load_img(std::string path);

            GPU_Image *image;
            std::shared_ptr<light::LightImage> light;

            int damages;
            float max_speed;
            float min_speed;
            float caliber;
            
            std::string name;
    };

#endif