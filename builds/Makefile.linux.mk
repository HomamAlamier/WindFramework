LINUX_CXX = g++
LINUX_LDFLAGS = -std=c++11 -fPIC
LINUX_LIBS = -DGLEW_STATIC -lGLEW -lGL -lGLU -lglfw -lX11 -lXrandr -pthread -ldl -lfreetype
LINUX_INCLUDE = -I../include -I/usr/include/freetype2
LINUX_SRC = ../Core/WindFramework.cpp
LINUX_SRC_CORE = $(shell find ../Core/ -type f -name '*.cpp')
LINUX_OUT_DIR = out/target/linux
LINUX_P_OUT = $(LINUX_OUT_DIR)/wind
LINUX_OUTPUT = $(LINUX_OUT_DIR)/libCore.so
LINUX_OBJDIR = .objs/target/linux/objs
LINUX_OBJS = $(LINUX_OBJDIR)/../Core/WindFramework.o
LINUX_OBJS_CORE = $(addprefix $(LINUX_OBJDIR)/,$(LINUX_SRC_CORE:.cpp=.o))


.PHONY: clean
$(LINUX_OBJS_CORE): $(LINUX_OBJDIR)/%.o: %.cpp
	mkdir -p $(@D)
	if [ $(DMODE) -eq 1 ]; then echo "DEBUG_MODE = 1" && $(LINUX_CXX) $(LINUX_LDFLAGS) -g -MMD -MP -c $< -o $@ -DGLFW_LIB $(ADD_DEFS) $(LINUX_INCLUDE); else echo "DEBUG_MODE = 0" && $(LINUX_CXX) $(LINUX_LDFLAGS) -MMD -MP -c $< -o $@ -DGLFW_LIB $(ADD_DEFS) $(LINUX_INCLUDE); fi
compile_resources:	
	#sh embedResources.sh
print_info:
	echo $(UNAME)
compile_linux: print_info $(LINUX_OBJS_CORE)
	mkdir -p $(LINUX_OUT_DIR)
	if [ $(DMODE) -eq 1 ]; then echo "LINK_DEBUG_MODE = 1" && $(LINUX_CXX) $(LINUX_LDFLAGS) -shared -g -o $(LINUX_OUTPUT) $(LINUX_OBJS_CORE) -DGLFW_LIB $(LINUX_LIBS); else echo "LINK_DEBUG_MODE = 0" && $(LINUX_CXX) $(LINUX_LDFLAGS) -shared -o $(LINUX_OUTPUT) $(LINUX_OBJS_CORE) -DGLFW_LIB $(LINUX_LIBS); fi
compile_test_linux:
	if [ $(DMODE) -eq 1 ]; then echo "DEBUG_MODE = 1" && $(LINUX_CXX) -std=c++11 -g -MMD -MP -c $(LINUX_SRC) -o $(LINUX_OBJS) -DGLFW_LIB $(ADD_DEFS) $(LINUX_INCLUDE); else echo "DEBUG_MODE = 0" && $(LINUX_CXX) -std=c++11 -MMD -MP -c $(LINUX_SRC) -o $(LINUX_OBJS) -DGLFW_LIB $(ADD_DEFS) $(LINUX_INCLUDE); fi
	if [ $(DMODE) -eq 1 ]; then echo "LINK_DEBUG_MODE = 1" && $(LINUX_CXX) -std=c++11 -g -o $(LINUX_P_OUT) $(LINUX_OBJS) -DGLFW_LIB -L$(LINUX_OUT_DIR) -lCore $(LINUX_LIBS); else echo "LINK_DEBUG_MODE = 0" && $(LINUX_CXX) -std=c++11 -o $(LINUX_P_OUT) $(LINUX_OBJS) -DGLFW_LIB -L$(LINUX_OUT_DIR) -lCore $(LINUX_LIBS); fi
