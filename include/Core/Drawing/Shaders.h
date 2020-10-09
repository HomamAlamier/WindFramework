#pragma once
#ifndef SHADERS_H
#define SHADERS_H
#include <GL.h>
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <Core/Drawing/Rectangle.h>

namespace shaders {

	class Shader{
		unsigned int id = 0;
		bool compiled = false;
	public:
		Shader(const char* name, const char* vertex, const char* frag);
		bool UniformMat4(const char* name, glm::mat4 value);
		bool UniformVec3(const char* name, glm::vec3 value);
		bool UniformInt(const char* name, int value);
		bool UniformBool(const char* name, bool value);
		bool UniformFloat(const char* name, float value);
		bool UniformRGBColor(const char* name, drawing::Color value);
		bool UniformRGBAColor(const char* name, drawing::Color value);
		bool Use();
		bool Vaild();
	};
	void InitAll();
	void Force_InitAll();
	Shader ColorShader();
	Shader FontShader();
	Shader GlyphShader();
}
#endif
