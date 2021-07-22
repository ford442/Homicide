#pragma once

#include <SDL2/SDL.h>
#include <iostream>

namespace event{
    class Keypad{
        public:
            Keypad();
            ~Keypad();

            void update(void);
            void event(SDL_Event* e);

            bool up(SDL_Scancode code);
            bool down(SDL_Scancode code);
            bool key(SDL_Scancode code);

        private:
            bool _up[UINT8_MAX];
            bool _down[UINT8_MAX];
            bool _key[UINT8_MAX];
    };
}