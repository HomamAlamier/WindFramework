cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall")
if (${ANDROID_PLATFORM_LEVEL} LESS 12)
    message(FATAL_ERROR "OpenGL 2 is not supported before API level 11 \
                      (currently using ${ANDROID_PLATFORM_LEVEL}).")
    return()
elseif (${ANDROID_PLATFORM_LEVEL} LESS 18)
    add_definitions("-DDYNAMIC_ES3")
    set(GL3STUB_SRC gl3stub.c)
    set(OPENGL_LIB GLESv2)
else ()
    set(OPENGL_LIB GLESv3)
endif (${ANDROID_PLATFORM_LEVEL} LESS 12)

include_directories(include)
include_directories(../../include)
add_library(libcore SHARED
        ${GL3STUB_SRC}
        libcore.cpp
        ../../Core/OSWindow/oswindow.cpp
        ../../Core/Drawing/Shaders.cpp
        ../../Core/Drawing/Font.cpp
        ../../Core/Drawing/Animation/ColorAnimation.cpp
        ../../Core/Drawing/Animation/AnimationBase.cpp
        ../../Core/Drawing/Animation/easeAnimation.cpp
        ../../Core/Drawing/Animation/Function.cpp
        ../../Core/Drawing/GraphicsElement.cpp
        ../../Core/Controls/Button.cpp
        ../../Core/Controls/BaseControl.cpp
        ../../Core/Controls/TextInput.cpp
        ../../Core/Controls/w_Timer.cpp
        ../../Core/Services/LogService.cpp
        ../../Core/Drawing/Image.cpp
        ../../Core/Controls/ImageView.cpp
        ../../Core/Controls/Menu.cpp
        ../../Core/Controls/ScrollBar.cpp)


add_library(libfreetype SHARED IMPORTED)
set_target_properties(libfreetype PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMAKE_ANDROID_ARCH_ABI}/libfreetype.so)


# Include libraries needed for gles3jni lib
target_link_libraries(libcore
        ${OPENGL_LIB}
        android
        EGL
        log
        m
        libfreetype)
