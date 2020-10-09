#!/bin/sh
if [ -e tools/ResourceCompiler ]
then
	echo "ResourceCompiler is exists !"
else
	echo "Compiling ResourceCompiler..."
	g++ --std=c++11 tools/Sources/ResourceCompiler.cpp -o tools/ResourceCompiler
fi

#Vars

export rcompiler="./tools/ResourceCompiler"

#Resource Files
export gles_Shader_ResourcesFile="es.h"
export gl_Shader_ResourcesFile="gl.h"
export Fonts_ResourcesFile="Fonts_ResourcesFile.h"

#Commands
rm -f $gl_Shader_ResourcesFile
$rcompiler -string -o $gl_Shader_ResourcesFile resources/shaders/ColorShader_Vertex.glsl \
resources/shaders/ColorShader_Fragment.glsl \
resources/shaders/FontShader_Vertex.glsl \
resources/shaders/FontShader_Fragment.glsl \
resources/shaders/GlyphShader_Vertex.glsl \
resources/shaders/GlyphShader_Fragment.glsl \
resources/shaders/ImageShader_Vertex.glsl \
resources/shaders/ImageShader_Fragment.glsl

