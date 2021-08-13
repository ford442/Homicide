#ifndef __PROJECTILE__TYPE__HPP__
    #define __PROJECTILE__TYPE__HPP__

    #include <iostream>
    #include <memory>
    #include <SDL2/SDL_gpu.h>

    class Projectile_type{
        public:
            Projectile_type();
            ~Projectile_type();

            GPU_Image *get_image(void) const;
            GPU_Image *get_icon(void) const;

            bool load(std::string path);

            int get_damages(void) const;
            float get_max_speed(void) const;
            float get_min_speed(void) const;
            float get_rand_speed(void) const;
            
            float get_caliber(void) const;

        private:

            bool load_img(std::string path);
            bool load_icon(std::string path);

            GPU_Image *image;
            GPU_Image *icon;

            int damages;
            float max_speed;
            float min_speed;
            float caliber;
            
            std::string name;
    };

#endif