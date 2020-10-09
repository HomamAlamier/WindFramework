#version 330 core
in vec4 color;
out vec4 FragColor;
uniform sampler2D img;
uniform int dmode;
void main(){
    if (dmode == 0) FragColor = color;
    else if (dmode == 1) FragColor = texture(img, color.xy);
}