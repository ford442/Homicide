#pragma once

#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include "events/Keypad.hpp"
#include "events/Mouse.hpp"

namespace event{
    class Handler{
        public:
            Handler();
            ~Handler();

            void event(SDL_Event* e);
            void update(void);

            bool IsKeyDown(SDL_Scancode key);
            bool IsKeyPush(SDL_Scancode key);
            bool IsKeyRelease(SDL_Scancode key);

            bool isButtonDown(MouseButtonEnum btn);
            bool isButtonPush(MouseButtonEnum btn);
            bool isButtonRelease(MouseButtonEnum btn);

            int mouse_x(void) const;
            int mouse_y(void) const;

            void mouse_pos(int *x, int *y);

        private:

            std::unique_ptr<Mouse> _mouse;
            std::unique_ptr<Keypad> _keypad;
    };
}