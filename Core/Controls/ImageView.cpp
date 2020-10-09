#include <Core/Controls/ImageView.h>

namespace controls {
    ImageView::ImageView(drawing::Rectangle rect, void* window) : BaseControl(rect, window) {
        ge->Clear();
    }
    void ImageView::Paint(int vpWidth, int vpHeight){
        ge->Draw(vpWidth, vpHeight);
    }
    void ImageView::OpenImage(const char* path){
        ge->Clear();
        auto img = new drawing::Image(drawing::Image::LoadFromFile(path));
        ge->AddImage(rect, img);
    }
    void ImageView::FromMemory(unsigned char* buffer, int len){
        ge->Clear();
        auto img = new drawing::Image(drawing::Image::LoadFromMemory(buffer, len));
        ge->AddImage(rect, img);
    }
}