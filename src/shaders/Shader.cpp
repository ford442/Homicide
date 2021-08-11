#include "shaders/Shader.hpp"
#include "main.hpp"

Shader::Shader(){
    frag = 0;
    vert = 0;
    shader = 0;
}

Shader::Shader(std::string frag, std::string vert){
    load(frag, vert);
}

Shader::~Shader(){
    unload();
}

Uint32 Shader::get_vert(void) const{
    return vert;
}

Uint32 Shader::get_frag(void) const{
    return frag;
}

Uint32 Shader::get_shader(void) const{
    return shader;
}

bool Shader::load_frag(std::string path){
    frag_path = path;
    return reload_frag();
}

bool Shader::load_vert(std::string path){
    vert_path = path;
    return reload_vert();
}

bool Shader::load(std::string frag, std::string vert){
    if (!load_frag(frag)) return false;
    if (!load_vert(vert)) return false;
    return true;
}

bool Shader::reload_frag(void){
    Uint32 frag = GPU_LoadShader(GPU_FRAGMENT_SHADER, frag_path.c_str());

    if (!frag){
        ERR("GPU_LoadShader fragment shader : " + frag_path)
        return false;
    }

    this->frag = frag;
    return true;
}

bool Shader::reload_vert(void){
    Uint32 vert = GPU_LoadShader(GPU_VERTEX_SHADER, vert_path.c_str());

    if (!vert){
        ERR("GPU_LoadShader vertex shader : " + vert_path)
        return false;
    }

    this->vert = vert;
    return true;
}

bool Shader::reload(void){
    if (!reload_frag()) return false;
    if (!reload_vert()) return false;
    return true;
}

bool Shader::set_sampler2d(GPU_Image *image){
    return true;
}

bool Shader::is_loaded(void){
    return (frag && vert && shader);
}

void Shader::unload(void){
    if (shader){
        GPU_FreeShader(shader);
        shader = 0;
    }
}

void Shader::update_shader_block(std::string position, std::string texCoord, std::string color, std::string viewModelMatrix){
    shader_block = GPU_LoadShaderBlock(shader, position.c_str(), texCoord.c_str(), color.c_str(), viewModelMatrix.c_str());
}

bool Shader::load_csv(std::string path){
    return true;
}

bool Shader::load_xml(XMLNode *node){
    return true;
}