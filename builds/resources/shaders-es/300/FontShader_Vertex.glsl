#version 300 es
#define PAD 0.001f
layout(location = 0) in vec4 vertex;
out vec2 TexCoords;
uniform float vpWidth;
uniform float vpHeight;
uniform float offsetX;
uniform float offsetY;
float _bet_m1_and_1(float value) { return value * 2.0f - 1.0f; }
void main()
{
    gl_Position = vec4(_bet_m1_and_1((vertex.x + offsetX) / vpWidth) + PAD, _bet_m1_and_1((vertex.y + offsetY) / vpHeight) + PAD, 0.0, 1.0);
    TexCoords = vertex.zw;
}

