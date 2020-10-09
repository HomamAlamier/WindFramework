#include <Core/Drawing/Image.h>
#define LOG_FN "image"
#include <Core/Services/LogService.h>
#define STB_IMAGE_IMPLEMENTATION
#include <Core/STB/stb_image.h>
namespace drawing
{
    Image::~Image(){
        if (gl_ptr != NULL) glDeleteTextures(1, &gl_ptr);
        gl_ptr = NULL;
    }
    void Image::glBind(){
        glBindTexture(GL_TEXTURE_2D, gl_ptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    Image Image::LoadFromFile(const char* path){
        Image x;
        Log_WriteLine(std::string("Loading image file ") + path + "...", LogType::Info, true);
        int w, h, nrc;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* img_data = stbi_load(path, &w, &h, &nrc, STBI_rgb_alpha);
        if (img_data) {
            std::string fb = "[ ";
            fb += std::to_string((int)img_data[0]) + ",";
            fb += std::to_string((int)img_data[1]) + ",";
            fb += std::to_string((int)img_data[2]) + ",";
            fb += std::to_string((int)img_data[3]) + " ]";
            Log_WriteLine("Orginal Image Components = " + std::to_string(nrc), LogType::Debug, true);
            Log_WriteLine("First 4 bytes of img_data " + fb, LogType::Debug, true);
            glGenTextures(1, &x.gl_ptr);
            glBindTexture(GL_TEXTURE_2D, x.gl_ptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
            glGenerateMipmap(GL_TEXTURE_2D);
            x.width = w;
            x.height = h;
        }
        else {
            Log_WriteLine("Error loading image file !", LogType::Error, true);
        }
        stbi_image_free(img_data);
        return x;
    }
    Image Image::LoadFromMemory(unsigned char* buffer, size_t len){
        Image x;
        Log_WriteLine("Loading image file from memory...", LogType::Info, true);
        int w, h, nrc;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* img_data = stbi_load_from_memory(buffer, len, &w, &h, &nrc, 0);
        if (img_data) {
            Log_WriteLine("nrc = " + std::to_string(nrc), LogType::Debug, true);
            glGenTextures(1, &x.gl_ptr);
            glBindTexture(GL_TEXTURE_2D, x.gl_ptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, nrc == 4 ? GL_RGBA : GL_RGB, w, h, 0, nrc == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, img_data);
            glGenerateMipmap(GL_TEXTURE_2D);
            x.width = w;
            x.height = h;
        }
        else {
            Log_WriteLine("Error loading image file !", LogType::Error, true);
        }
        stbi_image_free(img_data);
        return x;
    }
    bool Image::valid() { return gl_ptr != NULL; }
    int Image::Width() { return width; }
    int Image::Height() { return height; }
}
