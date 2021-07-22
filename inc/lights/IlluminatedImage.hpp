#pragma once

#include <iostream>
#include <SDL2/SDL_gpu.h>

namespace light{
    class IlluminatedImage{
        public:
            IlluminatedImage(void);
            ~IlluminatedImage();

            GPU_Image* get_image(void);
            void set_image(GPU_Image* image);
            bool load(std::string path);

            void blit(GPU_Target* t);

            void set_x(float x);
            void set_y(float y);
            void set_pos(float x, float y);

            float get_x(void);
            float get_y(void);
            float *get_x_ptr(void);
            float *get_y_ptr(void);
            void get_pos(float *x, float *y);
            void get_pos_ptr(float *x, float *y);

            void set_angle(float angle);
            float get_angle(void);
            float *get_angle_ptr(void);

            void set_center_x(float x);
            void set_center_y(float y);
            void set_center_pos(float x, float y);

            float get_center_x(void);
            float get_center_y(void);
            float *get_center_x_ptr(void);
            float *get_center_y_ptr(void);
            void get_center_pos(float *x, float *y);
            void get_center_pos_ptr(float *x, float *y);

            void set_scale(float scale);
            float get_scale(void);
            float *get_scale_ptr(void);

            void recenter(void);

        private:

            GPU_Image* _image;

            // image position, not camera
            float _x;
            float _y;
            
            float _angle;
            float _cx;
            float _cy;

            float _scale;

            void clear_members(void);
    };
}