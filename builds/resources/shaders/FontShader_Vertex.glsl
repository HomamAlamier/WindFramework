#version 330 core
#define PAD 0.001f
layout(location = 0) in vec4 vertex;
layout(location = 1) in vec2 vertex_attrib;
out vec2 TexCoords;
out vec2 hlat;
uniform float vpWidth;
uniform float vpHeight;
uniform float offsetX;
uniform float offsetY;
uniform float HLSI;
uniform float HLEI;
float _bet_m1_and_1(float value) { return value * 2 - 1; }
void main()
{
    gl_Position = vec4(_bet_m1_and_1((vertex.x + offsetX) / vpWidth) + PAD, _bet_m1_and_1((vertex.y + offsetY) / vpHeight) + PAD, 0.0, 1.0);
    TexCoords = vertex.zw;
    if (vertex_attrib.x >= HLSI && vertex_attrib.x <= HLEI) hlat.x = 1; else hlat.x = 0;
}

