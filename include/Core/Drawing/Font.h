#ifndef FONT_CONTAINER_H
#define FONT_CONTAINER_H


#include <vector>
#include <GL.h>
#include <Core/Drawing/GraphicsElement.h>
#include <Core/Drawing/Rectangle.h>
#include <string>


#include FT_FREETYPE_H 

namespace drawing {
    class TextBlock;
    struct F_Vector{
        float x = 0;
        float y = 0;
        F_Vector(float x, float y) { this->x = x; this->y = y; };
        F_Vector() {}
    };
    struct f_Char
    {
        bool valid;
        ::drawing::Rectangle rect;
        char32_t ch;
        long int advance;
        F_Vector bearing;
    };
    class fcc_coll{
        std::vector<f_Char> buf;
    public:
        void push_back(f_Char && _ch){
            buf.push_back(_ch);
        }
        f_Char* getPtr(char32_t ch){
            for (size_t i = 0; i < buf.size(); i++)
            {
                if (buf[i].ch == ch) return &buf[i];
            }  
            return NULL;
        }
        f_Char operator[](char32_t ch){
            for(auto fcc : buf){
                if (fcc.ch == ch){
                    return fcc;
                }
            }
            return {false};
        }
        void operator=(fcc_coll c){
            fcc_coll c2;
            this->buf = std::vector<f_Char>(c.buf);;
        }
    };
    class Font{
        unsigned int texture;
        fcc_coll fc_coll;
        signed long t_width, t_height;
        bool valid = false;
        long int f_height = 0, f_size = 0;
    public:
        Font(fcc_coll coll, unsigned int tex, signed long w, signed long h, long int adv, long int height, int size);
        Font();
        ~Font();
        bool Valid() { return valid; }
        int Height() { return f_height; }
        int Width(char32_t ch) { auto c = fc_coll[ch]; if (c.valid) return c.rect.width + 1; else return 0; }
        int Size() { return f_size; }
        f_Char* GetCharMetrics(char32_t ch);
        TextBlock RenderTextBlock(std::u32string text);
        void BindTextrue();
        F_Vector MeasureText(std::u32string text);
        static Font* GenerateFont(const char* fnt, uint32_t size);
    };
    class TextBlock{
        Font* font = NULL;
        uint32_t vbo = 0, vao = 0;
        int count = 0;
        bool valid = false;
        ::drawing::Rectangle rect;
    public:
        TextBlock(uint32_t vao, uint32_t vbo, int count, Font* font, ::drawing::Rectangle rect);
        TextBlock() {}
        ~TextBlock();
        void Render(int x, int y, int vpw, int vph, ::drawing::Color color);
        Font get_Font() { return *font; }
        F_Vector Size() { return {rect.width, rect.height}; }
    };
    class GLString{
        std::vector<f_Char*> chars;
        Font* fnt;
    public:
        GLString(Font* fnt);
        GLString(Font* fnt, std::u32string str);
        void insert(size_t pos, char32_t ch);
        void remove(size_t pos, size_t len);
        f_Char* operator[](int index);
        GLString operator+(char32_t r);
        GLString operator+(GLString r);
        void operator+=(GLString r);
        void operator+=(char32_t r);
        size_t length();
        size_t width();
        std::u32string u32string();
    };
}

#endif // !FONT_CONTAINER_H