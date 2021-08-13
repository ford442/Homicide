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

            virtual bool load(XMLNode *node);

        private:

            float _x, _y;
            float _w, _h;

        protected:

            bool is_mouse_over(void) const;
            event::Handler* events;
    };

#endif