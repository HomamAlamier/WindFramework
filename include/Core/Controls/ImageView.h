#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <Core/Controls/BaseControl.h>
#include <Core/Drawing/Image.h>
namespace controls {
    class ImageView : public BaseControl {
        drawing::Image* img = NULL;
        unsigned int VBO, VAO;
    public:
        ImageView(drawing::Rectangle rect, void* window);
        void Paint(int vpWidth, int vpHeight);
        void OpenImage(const char* path);
        void FromMemory(unsigned char* buffer, int len);
    };
}

#endif // !IMAGE_VIEW_H