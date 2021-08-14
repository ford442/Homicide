#ifndef __WIDGET_HPP__
    #define __WIDGET_HPP__

    #include <iostream>
    #include <SDL2/SDL_gpu.h>
    #include <SDL2/SDL.h>
    #include "events/Event_handler.hpp"
    #include "xml.hpp"
    class Widget{
        public:
            Widget();
            ~Widget();

            virtual void OnTick(const int delta);
            virtual void OnDraw(GPU_Target *t);
            virtual void OnHUDdraw(GPU_Target *t);
            virtual bool load(XMLNode *node);

            const float x(void) const;
            const float y(void) const;
            void pos(float *x, float *y) const;

            void x(const float x);
            void y(const float y);
            void pos(const float x, const float y);

            const float w(void) const;
            const float h(void) const;
            void size(float *w, float *h) const;

            void w(const float w);
            void h(const float h);
            void size(const float w, const float h);

            void set_events(event::Handler *handler);

            void set_window_size(int *w, int *h);

            bool is_mouse_hover(void);

        private:

            float _x, _y;
            float _w, _h;
            
            int *window_w, *window_h;

        protected:

            event::Handler* events;

            float str_to_float(std::string str);
            float str_to_int(std::string str);
    };

#endif