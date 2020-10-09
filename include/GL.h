#define TEST_RF_FONT



#ifndef __ANDROID__
    #ifndef GLEW_STATIC
        #define GLEW_STATIC
    #endif

    #define GLEW_STATIC
    #include <GL/glew.h>


    #include <GLFW/glfw3.h>

    //FONT_TEST

    #ifdef _WIN32
    #define FONT_TEST "C:\\windows\\fonts\\seguiemj.ttf"
    #else
    #define FONT_TEST "/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf"
    #endif // _WIN32


    #ifdef __APPLE__
    #include <ft2build.h>
    #else
        #include <freetype2/ft2build.h>
        #ifdef MSBUILD
            #pragma comment ( lib , "freetype" )
        #endif
    #endif // _WIN32
#else
    #if DYNAMIC_ES3
        #include "android/gl3stub.h"
    #else
        // Include the latest possible header file( GL version header )
        #if __ANDROID_API__ >= 24
            #include <GLES3/gl32.h>
        #pragma ____has_warning(">=24")
        #elif __ANDROID_API__ >= 21
            #include <GLES3/gl31.h>
        #else
            #include <GLES3/gl3.h>
        #endif
    #endif
    #include <ft2build.h>
#endif // !__ANDROID__
