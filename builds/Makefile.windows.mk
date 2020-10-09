WIN_CXX = i686-w64-mingw32-g++
WIN_LDFLAGS = -std=c++11 -fPIC -Wall -static-libgcc -static-libstdc++
WIN_LIBS = -DGLEW_STATIC -lglew32 -lopengl32 -lglu32 -lglfw3dll -lfreetype -lpng -lz
WIN_INCLUDE = -I../include -I/usr/i686-w64-mingw32/include/freetype2
WIN_SRC = ../Core/WindFramework.cpp
WIN_SRC_CORE = $(shell find ../Core/ -type f -name '*.cpp')
WIN_OUT_DIR = out/target/windows
WIN_P_OUT = $(WIN_OUT_DIR)/wind.exe
WIN_OBJDIR = .objs/target/windows/objs
WIN_OUTPUT = $(WIN_OUT_DIR)/libCore.dll
WIN_OBJS =  $(WIN_OBJDIR)/../Core/WindFramework.o
WIN_OBJS_CORE = $(addprefix $(WIN_OBJDIR)/,$(WIN_SRC_CORE:.cpp=.o))



.PHONY: clean
all: compile_linux
$(WIN_OBJS_CORE): $(WIN_OBJDIR)/%.o: %.cpp
	mkdir -p $(@D)
	if [ $(DMODE) -eq 1 ]; then echo "DEBUG_MODE = 1" && $(WIN_CXX) $(WIN_LDFLAGS) -g -MMD -MP -c $< -o $@ $(ADD_DEFS) $(WIN_INCLUDE); else echo "DEBUG_MODE = 0" && $(WIN_CXX) $(WIN_LDFLAGS) -MMD -MP -c $< -o $@ $(ADD_DEFS) $(WIN_INCLUDE); fi
compile_resources:	
	sh embedResources.sh
compile_windows: compile_resources $(WIN_OBJS_CORE)
	rm -f -R $(WIN_OUT_DIR)
	mkdir -p $(WIN_OUT_DIR)
	if [ $(DMODE) -eq 1 ]; then echo "LINK_DEBUG_MODE = 1" && $(WIN_CXX) $(WIN_LDFLAGS) -shared -g -o $(WIN_OUTPUT) $(WIN_OBJS_CORE) $(WIN_LIBS); else echo "LINK_DEBUG_MODE = 0" && $(WIN_CXX) $(WIN_LDFLAGS) -shared -o $(WIN_OUTPUT) $(WIN_OBJS_CORE) $(WIN_LIBS); fi
compile_test_windows:
	if [ $(DMODE) -eq 1 ]; then echo "DEBUG_MODE = 1" && $(WIN_CXX) $(WIN_LDFLAGS) -g -MMD -MP -c $(WIN_SRC) -o $(WIN_OBJS) $(ADD_DEFS) $(WIN_INCLUDE); else echo "DEBUG_MODE = 0" && $(WIN_CXX) $(WIN_LDFLAGS) -MMD -MP -c $(WIN_SRC) -o $(WIN_OBJS) $(ADD_DEFS) $(WIN_INCLUDE); fi
	if [ $(DMODE) -eq 1 ]; then echo "LINK_DEBUG_MODE = 1" && $(WIN_CXX) $(WIN_LDFLAGS) -g -o $(WIN_P_OUT) $(WIN_OBJS) -L$(WIN_OUT_DIR) -lCore $(WIN_LIBS); else echo "LINK_DEBUG_MODE = 0" && $(WIN_CXX) $(WIN_LDFLAGS) -o $(WIN_P_OUT) $(WIN_OBJS) -L$(WIN_OUT_DIR) -lCore $(WIN_LIBS); fi
	cp windows/bin/*.dll $(WIN_OUT_DIR)/
windows_7z: compile_windows compile_test_windows
	#7z a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=on windows.out.7z $(WIN_OUT_DIR)/*
	7z a -t7z -mx=9 -mfb=273 -ms -md=31 -myx=9 -mtm=- -mmt -mmtf -md=1536m -mmf=bt3 -mmc=10000 -mpb=0 -mlc=0 windows.out.7z $(WIN_OUT_DIR)/*
