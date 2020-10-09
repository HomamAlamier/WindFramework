#ifndef IMAGE_H
#define IMAGE_H

#include <GL.h>
#include <Core/Drawing/Rectangle.h>
namespace drawing {
    class Image {
        int width = 0, height = 0;
        unsigned int gl_ptr = NULL;
    public:
        ~Image();
        static Image LoadFromFile(const char* path);
        static Image LoadFromMemory(unsigned char* buffer, size_t len);
        drawing::Rectangle Rectangle() { return drawing::Rectangle(0, 0, width, height); }
        void glBind();
        bool valid();
        int Width();
        int Height();
    };
}
#endif // !IMAGE_H