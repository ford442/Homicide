#ifndef __BLUR__HPP__
    #define __BLUR__HPP__

    #include <iostream>
    #include "shaders/Shader.hpp"

    namespace shader{
        class Blur : public Shader{
            public:
                Blur();
                ~Blur();

                bool load_csv(std::string path) override;
                bool load_xml(XMLNode *node) override;
                bool set_sampler2d(GPU_Image *image) override;

                const float size(void) const;
                const float quality(void) const;
                const float direction(void) const;
                const SDL_FPoint resolution(void) const;

                void size(const float size);
                void quality(const float quality);
                void direction(const float direction);
                void resolution(const SDL_FPoint resolution);
                void resolution(const float width, const float height);

                void update_uniforms(void) override;
                bool update_uniforms_position(void) override;

            private:

                float _size;
                float _quality;
                float _direction;

                float resolution_w;
                float resolution_h;

                int size_uniform;
                int quality_uniform;
                int direction_uniform;
                int resolution_uniform;

                void set_size(std::string str);
                void set_quality(std::string str);
                void set_direction(std::string str);

        };
    }

#endif