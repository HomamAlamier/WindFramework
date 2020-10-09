#version 330 core
in vec2 TexCoords;
in vec2 hlat;
out vec4 FragColor;
uniform sampler2D text;
uniform vec3 textColor;
uniform bool coloredGlyph;
void main(){
    vec4 color;
    if (!coloredGlyph){
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        color = vec4(textColor, 1.0) * sampled;
    }
    else{
        color = texture(text, TexCoords).rgba;
    }
    if (hlat.x == 1){
        FragColor = vec4(color.rgb * vec3(0.0, 1.0, 0.0), 1.0 - color.a);// else FragColor = color;
        if (FragColor.a <= 0.1) FragColor = color;
    }
    else FragColor = color;
    //FragColor = color;
}