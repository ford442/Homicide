#ifndef __SHADER__HPP__
    #define __SHADER__HPP__

    #include <iostream>
    #include <SDL2/SDL_gpu.h>
    #include "xml.hpp"

    class Shader{
        public:
            Shader();
            Shader(std::string frag, std::string vert);
            ~Shader();

            virtual bool load_csv(std::string path);
            virtual bool load_xml(XMLNode *node);

            Uint32 get_vert(void) const;
            Uint32 get_frag(void) const;
            Uint32 get_shader(void) const;

            bool load_frag(std::string path);
            bool load_vert(std::string path);
            bool load(std::string frag, std::string vert);

            bool reload_frag(void);
            bool reload_vert(void);
            bool reload(void);

            virtual bool set_sampler2d(GPU_Image *image);

            bool is_loaded(void);
            void unload(void);

            virtual void update_uniforms(void);
            virtual bool update_uniforms_position(void);

            void active(void);
            void unacitve(void);

            std::string get_frag_path(void) const;
            std::string get_vert_path(void) const;

        private:

            std::string frag_path;
            std::string vert_path;

            Uint32 vert;
            Uint32 frag;
            Uint32 shader;

            GPU_ShaderBlock shader_block;

        protected:
            virtual void update_shader_block(std::string position, std::string texCoord, std::string color, std::string viewModelMatrix);
    };

#endif