#include "shaders/Blur.hpp"
#include "csv.hpp"
#include "main.hpp"

using namespace shader;

#define STANDART_SIZE_VALUE 8
#define STANDART_QUALITY_VALUE 4
#define STANDART_DIRECTION_VALUE 16

Blur::Blur() : Shader(){
    _size = STANDART_SIZE_VALUE;
    _quality = STANDART_QUALITY_VALUE;
    _direction = STANDART_DIRECTION_VALUE;
}

Blur::~Blur(){

}

bool Blur::load_csv(std::string path){
    CSV::Document doc;
    if (!doc.load(path)) return false;

    std::string value = doc.search("size");

    if (!value.empty()){
        try{
            size(std::stoi(value));
        } catch (std::exception &e){
            ERR("standart excpetion : " + std::string(e.what()));
            size(STANDART_SIZE_VALUE);
        }
    } else {
        WARN("cannot found the node \"size\" in \"" + path + "\"");
        size(STANDART_SIZE_VALUE);
    }

    value = doc.search("quality");

    if (!value.empty()){
        try{
            quality(std::stoi(value));
        } catch (std::exception &e){
            ERR("standart excpetion : " + std::string(e.what()));
            quality(STANDART_QUALITY_VALUE);
        }
    } else {
        WARN("cannot found the node \"size\" in \"" + path + "\"");
        quality(STANDART_QUALITY_VALUE);
    }

    value = doc.search("direction");

    if (!value.empty()){
        try{
            direction(std::stoi(value));
        } catch (std::exception &e){
            ERR("standart excpetion : " + std::string(e.what()));
            direction(STANDART_DIRECTION_VALUE);
        }
    } else {
        WARN("cannot found the node \"size\" in \"" + path + "\"");
        direction(STANDART_DIRECTION_VALUE);
    }

    std::string frag = doc.search("fragement_shader");
    std::string vert = doc.search("vertex_shader");

    if (frag.empty()){
        ERR("cannot load the shader, the fragement shader path is missing in \"" + path + "\"");
        return false;
    }

    if (vert.empty()){
        ERR("cannot load the shader, the vertex shader path is missing in \"" + path + "\"");
        return false;
    }

    if (!load(frag, vert)){
        ERR("error occured durring the laoding the the shader files");
        return false;
    }

    update_shader_block("Position", "TexCoord", "Color", "ViewModelMatrix");
    return true;
}

bool Blur::load_xml(XMLNode *node){
    for (int c=0; c<node->children.size; c++){
        XMLNode *child = XMLNode_child(node, c);

        if (is_equal(child->tag, "size")){
            for (int a=0; a<child->attributes.size; a++){
                XMLAttribute attr = child->attributes.data[a];

                if (is_equal(attr.key, "value")){
                    set_size(attr.value);
                }
            }
        } else if (is_equal(child->tag, "quality")){
            for (int a=0; a<child->attributes.size; a++){
                XMLAttribute attr = child->attributes.data[a];

                if (is_equal(attr.key, "value")){
                    set_quality(attr.value);
                }
            }
        } else if (is_equal(child->tag, "direction")){
            for (int a=0; a<child->attributes.size; a++){
                XMLAttribute attr = child->attributes.data[a];

                if (is_equal(attr.key, "value")){
                    set_direction(attr.value);
                }
            }
        } else if (is_equal(child->tag, "fragement")){
            for (int a=0; a<child->attributes.size; a++){
                XMLAttribute attr = child->attributes.data[a];

                if (is_equal(attr.key, "path")){
                    if (!load_frag(attr.value)) return false;
                }
            }
        } else if (is_equal(child->tag, "vertex")){
            for (int a=0; a<child->attributes.size; a++){
                XMLAttribute attr = child->attributes.data[a];

                if (is_equal(attr.key, "path")){
                    if (!load_vert(attr.value)) return false;
                }
            }
        }
    }

    return true;
}

bool Blur::set_sampler2d(GPU_Image *image){
    Uint32 texture = GPU_GetAttributeLocation(get_shader(), "Texture");
    if (!texture) return false;

    GPU_SetShaderImage(image, texture, 2);
    return true;
}

const float Blur::size(void) const{
    return _size;
}

const float Blur::quality(void) const{
    return _quality;
}

const float Blur::direction(void) const{
    return _direction;
}

void Blur::size(const float size){
    _size = size;
}

void Blur::quality(const float quality){
    _quality = quality;
}

void Blur::direction(const float direction){
    _direction = direction;
}

void Blur::set_size(std::string str){
    try {
        size(std::stoi(str));
    } catch (std::exception &e){
        ERR("standart exception" + std::string(e.what()));
        size(STANDART_SIZE_VALUE);
    }
}

void Blur::set_quality(std::string str){
    try {
        quality(std::stoi(str));
    } catch (std::exception &e){
        ERR("standart exception" + std::string(e.what()));
        quality(STANDART_QUALITY_VALUE);
    }
}

void Blur::set_direction(std::string str){
    try {
        direction(std::stoi(str));
    } catch (std::exception &e){
        ERR("standart exception" + std::string(e.what()));
        direction(STANDART_DIRECTION_VALUE);
    }
}

void Blur::update_uniforms(void){
    GPU_SetUniformf(size_uniform, size());
    GPU_SetUniformf(quality_uniform, quality());
    GPU_SetUniformf(direction_uniform, direction());
    float resolution[2] = {resolution_w, resolution_h};
    GPU_GetUniformfv(resolution_uniform, 2, resolution);
} 

bool Blur::update_uniforms_position(void){
    size_uniform = GPU_GetUniformLocation(get_shader(), "iSize");
    if (size_uniform == -1){
        ERR("cannot found the uniform \"iSize\" in \"" + get_frag_path() + "\"");
        return false;
    }

    quality_uniform = GPU_GetUniformLocation(get_shader(), "iQuality");
    if (quality_uniform == -1){
        ERR("cannot found the uniform \"iQuality\" in \"" + get_frag_path() + "\"");
        return false;
    }

    direction_uniform = GPU_GetUniformLocation(get_shader(), "iDirection");
    if (direction_uniform == -1){
        ERR("cannot found the uniform \"iDirection\" in \"" + get_frag_path() + "\"");
        return false;
    }

    resolution_uniform = GPU_GetUniformLocation(get_shader(), "iResolution");
    if (resolution_uniform == -1){
        ERR("cannot found the uniform \"iResolution\" in \"" + get_frag_path() + "\"");
        return false;
    }
    return true;
}

void Blur::resolution(const SDL_FPoint resolution){
    this->resolution(resolution.x, resolution.y);
}

void Blur::resolution(const float width, const float height){
    resolution_w = width;
    resolution_h = height;
}