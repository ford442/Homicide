#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>

#include "lights/LightImage.hpp"

namespace light{
    class LightSource{
        public:
            LightSource();
            ~LightSource();

            enum Shadow_calculation{
                Shadow_pylogon,
                Shadow_image,
            };

            const float x(void) const;
            const float y(void) const;
            void pos(int *x, int *y) const;

            void x(const float x);
            void y(const float y);
            void pos(const float x, const float y);

            SDL_Color color(void) const;
            SDL_Color* color_ptr(void);
            void color(SDL_Color color);

            std::shared_ptr<light::LightImage> get_image(void) const;
            void set_image(std::shared_ptr<light::LightImage> image);

            std::shared_ptr<light::LightImage> get_calculation_image(void) const;
            void set_calculation_image(std::shared_ptr<light::LightImage> image);

            bool is_on(void) const;

            void active(void);
            void desactive(void);

            float angle(void);
            void angle(const float angle);

            std::vector<std::tuple<float, float, float>> vecVisibilityPolygonPoints;

            Shadow_calculation get_calculation(void) const;
            void set_calculation(Shadow_calculation type);
        
        private:
            float _x, _y;
            bool _isOn;
            float _angle;
            
            SDL_Color _color;
            std::shared_ptr<light::LightImage> _image;
            std::shared_ptr<light::LightImage> _calculation_image;
            Shadow_calculation calculation_type;
    };
}