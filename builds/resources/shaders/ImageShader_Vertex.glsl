#version 330 core
#define PAD 0.001f
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexC;
out vec2 texC;
uniform float vpWidth;
uniform float vpHeight;
float _bet_m1_and_1(float value) {return value * 2 - 1;}
void main(){
    vec3 v = vec3(_bet_m1_and_1(aPos.x / vpWidth) + PAD, -_bet_m1_and_1(aPos.y / vpHeight) + PAD, 0.0f);
    gl_Position = vec4(v, 1.0);
    texC = aTexC;
}
