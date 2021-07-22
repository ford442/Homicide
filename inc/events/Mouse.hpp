#pragma once

#include <SDL2/SDL.h>
#include <iostream>

namespace event{
    class Mouse{
        public:
            Mouse();
            ~Mouse();

            struct MouseButton{
                bool left;
                bool middle;
                bool right;
            };

            MouseButton up(void);
            MouseButton btn(void);
            MouseButton down(void);

            void update(void);

            void event(SDL_Event* e);

            int x(void);
            int y(void);

            SDL_Point pos(void);
            void pos(int* x, int* y);

            bool wheel_up(void);
            bool wheel_down(void);

        private:
            int _x, _y;

            MouseButton _up;
            MouseButton _btn;
            MouseButton _down;

            bool _wheel_up, _wheel_down;

    };

    enum MouseButtonEnum{
        Mouse_button_left,
        Mouse_button_middle,
        Mouse_button_right
    };
}