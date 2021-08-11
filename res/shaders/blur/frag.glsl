#version 330 core

out vec4 FragColor;
in vec2 fragCoord

uniform float size;
uniform float qualtity;
uniform float direction;
uniform vec2 iResolution;

uniform sampler2D texture;

void main(){
    float Pi = 6.28318530718; // Pi*2

    vec2 Radius = size/iResolution.xy;

    vec2 uv = gl_Position/iResolution.xy;
    vec4 Color = texture2D(texture, uv);
    
    // Blur calculations
    for( float d=0.0; d<Pi; d+=Pi/directions){
		for(float i=1.0/quality; i<=1.0; i+=1.0/quality){

			Color += texture2D(texture, uv+vec2(cos(d),sin(d))*Radius*i);		
        }
    }
    
    // Output to screen
    Color /= quality * directions - 15.0;
    FragColor =  Color;
}