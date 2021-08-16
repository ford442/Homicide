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
            bool _up[SDL_NUM_SCANCODES];
            bool _down[SDL_NUM_SCANCODES];
            bool _key[SDL_NUM_SCANCODES];
    };
}