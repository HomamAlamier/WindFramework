#version 330 core
#define PAD 0.001f
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 attrib;
out vec4 color;
uniform float vpWidth;
uniform float vpHeight;
float _bet_m1_and_1(float value) {return value * 2 - 1;}
void main(){
    vec3 v = vec3(_bet_m1_and_1(aPos.x / vpWidth) + PAD, -_bet_m1_and_1(aPos.y / vpHeight) + PAD, aPos.z);
    gl_Position = vec4(v, 1.0);
    color = vec4(aColor, attrib.x);
}
