#pragma once

#include <SDL2/SDL_gpu.h>

class Camera{
    public:
        Camera();
        ~Camera();

        enum CameraMovementType{
            Camera_FadeIn,
            Camera_FadeOut,
            Camera_Linear
        };

        void setMovementType(CameraMovementType type);

        void OnTick(const int delta);
        void OnDraw(GPU_Target *t);
        void go_to(float x, float y);
        void zoom(float zoom);

        void get_pos(float *x, float *y);
        void get_zoom(float *zoom);

        void set_delay(int delay);

    private:
        float _x;
        float _y;
        float _zoom;

        // movement
        bool _moving;
        float _target_x;
        float _target_y;
        float _target_zoom;

        int _ticks;
        int _delay;

        float _coef;

        CameraMovementType movement_type;

        float _dif_x;
        float _dif_y;
        float _dif_zoom;

        float _prev_x;
        float _prev_y;
        float _prev_zoom;

        void update_dif(void);
        bool should_move(void) const;
};