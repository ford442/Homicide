#ifndef __LIGHTSOURCE_HPP__
    #define __LIGHTSOURCE_HPP__

    #include <iostream>
    #include <vector>
    #include <SDL2/SDL_gpu.h>

    #include "ShadowCaster.hpp"

    namespace light{
        class LightSource{
            public:
                LightSource();
                ~LightSource();

                const int x(void) const;
                const int y(void) const;
                void pos(int *x, int *y) const;

                void x(const int x);
                void y(const int y);
                void pos(const int x, const int y);

                void on(void);
                void off(void);
                bool is_on(void) const;
    
                void OnTick(void);

                /**
                 * @brief lock the polygone calculation on the actual result
                 * 
                 */
                void lock(void);
                void unlock(void);
                bool is_locked(void) const;

                bool update_size(const int w, const int h);
                void update_visibility_poly(ShadowCaster* shadowCaster);

                std::vector<ShadowCaster::Visibility_poly_point> get_vibility_poly(void) const;

            private:

                void destroy_target(void);
                GPU_Target *target;
                GPU_Image *image;

                std::vector<ShadowCaster::Visibility_poly_point> visibility_poly;

                float _x, _y;
                float _w, _h;

                bool active;
                bool moved;
                bool locked;

                void update_target(void);
                
        };
    }

#endif