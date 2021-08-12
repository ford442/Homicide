#ifndef __LIGHTSOURCE_HPP__
    #define __LIGHTSOURCE_HPP__

    #include <iostream>
    #include <SDL2/SDL_gpu.h>

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

            private:

                void destroy_target(void);
                GPU_Target *target;
                GPU_Image *image;

                float _x, _y;

                bool active;
                bool moved;
                bool locked;
        };
    }

#endif