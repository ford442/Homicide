#include "widgets/Text.hpp"
#include "main.hpp"

Text::Text(){
    font = nullptr;
    image = nullptr;
}

Text::~Text(){
    font = nullptr;
    image = nullptr;
}

bool Text::set_text(std::string){
    this->text = text; 
    return update();
}

bool Text::set_forground_color(SDL_Color color){
    fg = color;
    return update();
}

bool Text::set_background_color(SDL_Color color){
    bg = color;
    return update();
}

bool Text::set_font(Font *font){
    this->font = font;
    return update();
}

bool Text::set_render_type(Font::Render_type type){
    render_type = type;
    return update();
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

    return image;
}

void Text::OnDraw(GPU_Target *t){
    GPU_Blit(image, nullptr, t, x(), y());
}

GPU_Image *Text::get_image(void) const{
    return image;
}