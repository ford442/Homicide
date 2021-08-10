#ifndef __SHADER__HPP__
    #define __SHADER__HPP__

    #include <iostream>
    #include <SDL2/SDL_gpu.h>

    class Shader{
        public:
            Shader();
            Shader(std::string frag, std::string vert);
            ~Shader();

            Uint32 get_vert(void) const;
            Uint32 get_frag(void) const;
            Uint32 get_shader(void) const;

            bool load_frag(std::string path);
            bool load_vert(std::string path);
            bool load(std::string frag, std::string vert);

            bool reload_frag(void);
            bool reload_vert(void);
            bool reload(void);

            virtual bool set_sampler2d(void);

            bool is_loaded(void);
            void unload(void);

        private:

            std::string frag_path;
            std::string vert_path;

            Uint32 vert;
            Uint32 frag;
            Uint32 shader;

            GPU_ShaderBlock shader_block;

        protected:
        
            virtual bool update_shader_block(void);
    };

#endif