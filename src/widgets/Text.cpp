#include "widgets/Text.hpp"
#include "main.hpp"
#include <sstream>

Text::Text(){
    font = nullptr;
    image = nullptr;
    render_type = Font::RenderType_Solid;
    set_forground_color({255, 255, 255, 255});
    scale(1);
}

Text::~Text(){
    font = nullptr;
    image = nullptr;
}

void Text::set_text(std::string){
    this->text = text;
}

void Text::set_forground_color(SDL_Color color){
    fg = color;
}

void Text::set_background_color(SDL_Color color){
    bg = color;
}

void Text::set_font(std::shared_ptr<Font> font){
    this->font = font;
}

void Text::set_render_type(Font::Render_type type){
    render_type = type;
}

bool Text::update(void){
    if (!font){
        ERR("cannot update a text without a valid font");
        return false;
    }

    if (image){
        GPU_FreeImage(image);
        image = nullptr;
    }

    switch (render_type){
        case Font::RenderType_Solid:
            image = font->solid_image(text, fg);
            break;
        
        case Font::RenderType_Shaded:
            image = font->shaded_image(text, fg, bg);
            break;
        
        case Font::RenderType_Blended:
            image = font->blended_image(text, fg);
            break;
        
        default:
            break;
    }

    if (!image) return false;
    if (scale() > 1) GPU_SetImageFilter(image, GPU_FILTER_NEAREST);    

    return true;
}

void Text::OnDraw(GPU_Target *t){
    GPU_BlitScale(image, nullptr, t, x(), y(), scale(), scale());
}

GPU_Image *Text::get_image(void) const{
    return image;
}

static inline SDL_Color colorConverter(int hexValue){
    SDL_Color rgbColor;
    rgbColor.r = (hexValue >> 24) & 0xFF;
    rgbColor.g = (hexValue >> 16) & 0xFF;
    rgbColor.b = (hexValue >> 8)& 0xFF;
    rgbColor.a = hexValue & 0xff;

    return rgbColor;
}

static inline void convert_hexa_to_Uint32(std::string hex_src, Uint32 *dst){
    std::stringstream ss;
    ss << std::hex << hex_src;
    ss >> *dst;
}

static inline SDL_Color str_to_rgb(std::string str){
    Uint32 hex;
    convert_hexa_to_Uint32(str, &hex);
    return colorConverter(hex);
}

bool Text::load(XMLNode *node){
    LOG("load a text widget");
    int font_size = -1;
    for (int a=0; a<node->attributes.size; a++){
        XMLAttribute attr = node->attributes.data[a];

        if (is_equal(attr.key, "x")){
            try {
                x(std::stof(attr.value));
            } catch (std::exception &e){
                ERR("standart exception : " + std::string(e.what()));
                return false;
            }
        } else if (is_equal(attr.key, "y")){
            try {
                y(std::stof(attr.value));
            } catch (std::exception &e){
                ERR("standart exception : " + std::string(e.what()));
                return false;
            }
        } else if (is_equal(attr.key, "bg")){
            bg = str_to_rgb(attr.value);

        } else if (is_equal(attr.key, "fg")){
            fg = str_to_rgb(attr.value);

        } else if (is_equal(attr.key, "type")){
            if (is_equal(attr.value, "solid")){
                render_type = Font::RenderType_Solid;

            } else if (is_equal(attr.value, "shaded")){
                render_type = Font::RenderType_Shaded;

            } else if (is_equal(attr.value, "blended")){
                render_type = Font::RenderType_Blended;

            } else {
                WARN("cannot reconize \"" + std::string(attr.value) + "\" text type, set as solid");
                render_type = Font::RenderType_Solid;
            }

        } else if (is_equal(attr.key, "text")){
            text = attr.value;
        
        } else if (is_equal(attr.key, "font")){
            font = ttf->get(attr.value);

            if (!font){
                font = ttf->get_fonts().front();
                WARN("cannot found \"" + std::string(attr.value) + "\" font, set as : " + font->get_name());
            }

            if (font_size > 0){
                font->set_size(font_size);
            }
        
        } else if (is_equal(attr.key, "size")){
            try {
                font_size = std::stoi(attr.value);

                if (font){
                    font->set_size(font_size);
                    font_size = -1;
                }

            } catch(std::exception &e){
                ERR("standart exception : " + std::string(e.what()));
                font_size = -1;
            }
            
        
        } else if (is_equal(attr.key, "scale")){

            try {
                scale(std::stof(attr.value));
            } catch (std::exception &e){
                ERR("standart exception : " + std::string(e.what()));
                scale(1);
            }

        } else {
            WARN("cannot reconize \"" + std::string(attr.key) + "\" text attribute");
        }
    }
    LOG("widget loaded, update the image");
    return update();
}


void Text::OnHUDdraw(GPU_Target *t){

}

void Text::set_ttf(TTF* ttf){
    this->ttf = ttf;
}

const float Text::scale(void) const{
    return _scale;
}

void Text::scale(const float scale){
    _scale = scale;
}