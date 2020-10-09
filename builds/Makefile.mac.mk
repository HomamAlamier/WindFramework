MAC_CXX = g++
MAC_LDFLAGS = -std=c++11 -fPIC
MAC_LIBS = -Lmac/libs -lglew -lglfw3 -lfreetype -framework Cocoa -framework OpenGL -framework IOKit
MAC_INCLUDE = -I../include -Imac/include
MAC_SRC = ../Core/WindFramework.cpp
MAC_SRC_CORE = $(shell find ../Core/ -type f -name '*.cpp')
MAC_OUT_DIR = out/target/darwin
MAC_OBJDIR = .objs/target/darwin/objs
MAC_P_OUT = $(MAC_OUT_DIR)/wind
MAC_OUTPUT = $(MAC_OUT_DIR)/libCore.so
MAC_OBJS = $(MAC_OBJDIR)/../Core/WindFramework.o
MAC_OBJS_CORE =  $(addprefix $(MAC_OBJDIR)/,$(MAC_SRC_CORE:.cpp=.o))


.PHONY: clean
$(MAC_OBJS_CORE): $(MAC_OBJDIR)/%.o: %.cpp
	mkdir -p $(@D)
	if [ $(DMODE) -eq 1 ]; then echo "DEBUG_MODE = 1" && $(MAC_CXX) $(MAC_LDFLAGS) -g -MMD -MP -c $< -o $@ $(ADD_DEFS) $(MAC_INCLUDE); else echo "DEBUG_MODE = 0" && $(MAC_CXX) $(MAC_LDFLAGS) -MMD -MP -c $< -o $@ $(ADD_DEFS) $(MAC_INCLUDE); fi
compile_resources:	
	sh embedResources.sh
print_info:
	echo $(UNAME)
compile_mac: print_info compile_resources $(MAC_OBJS_CORE)
	mkdir -p $(MAC_OUT_DIR)
	if [ $(DMODE) -eq 1 ]; then echo "LINK_DEBUG_MODE = 1" && $(MAC_CXX) $(MAC_LDFLAGS) -shared -g -o $(MAC_OUTPUT) $(MAC_OBJS_CORE) $(MAC_LIBS); else echo "LINK_DEBUG_MODE = 0" && $(MAC_CXX) $(MAC_LDFLAGS) -shared -o $(MAC_OUTPUT) $(MAC_OBJS_CORE) $(MAC_LIBS); fi
compile_test_mac:
	if [ $(DMODE) -eq 1 ]; then echo "DEBUG_MODE = 1" && $(MAC_CXX) -std=c++11 -g -MMD -MP -c $(MAC_SRC) -o $(MAC_OBJS) $(ADD_DEFS) $(MAC_INCLUDE); else echo "DEBUG_MODE = 0" && $(MAC_CXX) -std=c++11 -MMD -MP -c $(MAC_SRC) -o $(MAC_OBJS) $(ADD_DEFS) $(MAC_INCLUDE); fi
	if [ $(DMODE) -eq 1 ]; then echo "LINK_DEBUG_MODE = 1" && $(MAC_CXX) -std=c++11 -g -o $(MAC_P_OUT) $(MAC_OBJS) -L$(MAC_OUT_DIR) -lCore $(MAC_LIBS); else echo "LINK_DEBUG_MODE = 0" && $(MAC_CXX) -std=c++11 -o $(MAC_P_OUT) $(MAC_OBJS) -L$(MAC_OUT_DIR) -lCore $(MAC_LIBS); fi
