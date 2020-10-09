include Makefile.linux.mk
include Makefile.windows.mk
include Makefile.mac.mk
clean:
	rm -f -R .objs/*
	rm -f -R out/*
	
shaders_rf:
	./tools/ResourceCompiler -string -app -o shaders.h resources/shaders/ColorShader_Vertex.glsl \
	resources/shaders/ColorShader_Fragment.glsl \
	resources/shaders/FontShader_Vertex.glsl \
	resources/shaders/FontShader_Fragment.glsl \
	resources/shaders/GlyphShader_Vertex.glsl \
	resources/shaders/GlyphShader_Fragment.glsl \
	resources/shaders/ImageShader_Vertex.glsl \
	resources/shaders/ImageShader_Fragment.glsl
	./tools/ResourceCompiler -binary -o images.h resources/img.png
	cp images.h ../include/ResourceHeaders/Images_RF.h
	cp shaders.h ../include/ResourceHeaders/GL_shaders.h
	rm -f shaders.h
	rm -f images.h
	
windows: shaders_rf compile_windows compile_test_windows
linux: shaders_rf compile_linux compile_test_linux
mac: shaders_rf compile_mac compile_test_mac


