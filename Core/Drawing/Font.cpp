#include <Core/Drawing/Font.h>
#define LOG_FN "fontc"
#include <Core/Services/LogService.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Core/STB/stb_image_write.h>
#include <Core/Drawing/Shaders.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Drawing/GraphicsElement.h>
namespace drawing {
    Font::Font() {
        valid = false;
    }
    Font::Font(fcc_coll coll, unsigned int tex, signed long w, signed long h, long int adv, long int height, int size) {
        fc_coll = fcc_coll(coll);
        texture = tex;
        t_width = w;
        t_height = h;
        f_height = height;
        f_size = size;
        valid = true;
    }
    Font::~Font(){
        glDeleteTextures(1, &texture);
        texture = 0;
        //fc_coll.~fcc_coll();
    }
    Font* Font::GenerateFont(const char* fnt, uint32_t size) {
        FT_Library lib;
        FT_Face face;
        if (FT_Init_FreeType(&lib)) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            Log_WriteLine("Could not init FreeType Library", LogType::Error, true);
            return new Font();
        }
    
        Log_WriteLine("Loading font " + std::string(fnt) + "...", LogType::Info, true);
        #ifdef TEST_RF_FONT
        #include <ResourceHeaders/Fonts_ResourcesFile.h>
        Log_WriteLine("RF Font size = " + std::to_string(sizeof(font_bytes)), LogType::Info, true);
        /*std::fstream str("font.ttf", std::ios::out);
        str.write((char*)font_bytes, sizeof(font_bytes));
        str.flush();
        str.close();*/
        if (FT_New_Memory_Face(lib, font_bytes, sizeof(font_bytes), 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            Log_WriteLine("Failed to load font", LogType::Error, true);
            return new Font();
        }
        #else
        if (FT_New_Face(lib, fnt, 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            Log_WriteLine("Failed to load font", LogType::Error, true);
            return Font();
        }
        #endif
        FT_Select_Charmap(face, ft_encoding_unicode);
        FT_Set_Pixel_Sizes(face, 0, size);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        signed long allW = (face->size->metrics.height >> 6) * 85,
            allH = (face->size->metrics.height >> 6);
        
        Log_WriteLine("Texture Size = " + std::to_string(allW) + "x" + std::to_string(allH), LogType::Info, true);
        unsigned int tex, fb;
    
        Log_WriteLine("Generating FrameBuffer...", LogType::Info, true);
        //Gen FrameBuffer
        glGenFramebuffers(1, &fb); 
        Log_WriteLine("FrameBuffer id = " + std::to_string(fb), LogType::Info, true);
        glBindFramebuffer(GL_FRAMEBUFFER, fb);      
        //Gen Texture
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, allW, allH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
        //connecting texture with framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

        GLenum err;
        
        if ((err = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE){
            std::string errstr;
            switch(err){
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: errstr = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
                case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: errstr = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
                //case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: errstr = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: errstr = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: errstr = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
                //case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: errstr = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
            }
            Log_WriteLine(std::string("glCheckFramebufferStatus didn't return GL_FRAMEBUFFER_COMPLETE ( ") + errstr + " )!. deleting framebuffer..", LogType::Error, true);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDeleteBuffers(1, &fb);  
            return new Font();
        }  
        glBindTexture(GL_TEXTURE_2D, 0);
        
        Log_WriteLine("Preloading Chars...", LogType::Info, true);
        glViewport(0, 0, allW, allH);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        fcc_coll chars;
        /// Render


        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        auto fs = shaders::GlyphShader();
        fs.Use();
        fs.UniformRGBColor("textColor", drawing::Color::White());
        glm::mat4 projection = glm::ortho(0.0f, (float)allW, (float)allH, 0.0f);
        fs.UniformMat4("proj", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        float x = 0;
        for (char32_t c = 0; c < 128; c++)
        {
            bool colored = false;
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_DEFAULT | FT_LOAD_COLOR))
            {
                Log_WriteLine("Failed to load Glyph (char=" + std::to_string(c) + ")", LogType::Error, true);
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)) {
                Log_WriteLine("Failed to render Glyph (char=" + std::to_string(c) + ")", LogType::Error, true);
                std::cout << "ERROR::FREETYTPE: Failed to render Glyph" << std::endl;
                continue;
            }
            if (face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_BGRA) {
                std::cout << "Glyph is colored !" << std::endl;
                colored = true;
            }
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            float xpos = x;
            float ypos = 0;
            float w = face->glyph->bitmap.width;
            float h = face->glyph->bitmap.rows;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            glBindTexture(GL_TEXTURE_2D, texture);
            // update content of VBO memory
            //glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            //glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            fs.UniformBool("coloredGlyph", colored);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindTexture(GL_TEXTURE_2D, 0);
            glDeleteTextures(1, &texture);



            chars.push_back(
                {
                    true,
                    drawing::Rectangle(xpos, ypos, w, h),
                    c,
                    face->glyph->advance.x >> 6,
                    F_Vector(face->glyph->bitmap_left, face->glyph->bitmap_top)
                });
                
            x += face->glyph->bitmap.width + 1;
        }
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);

        /// /Render
        /*glPixelStorei(GL_PACK_ALIGNMENT, 1);
        GLubyte* pixels = new GLubyte[allW * allH * 4];
        glReadPixels(0, 0, allW, allH, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        Log_WriteLine("Writing pixels to test.bmp...", LogType::Info, true);

        stbi_write_bmp("/sdcard/test.bmp", allW, allH, 4, pixels);*/

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &fb);
        long int adv = face->glyph->advance.x >> 6, f_h = face->size->metrics.height >> 6;
        FT_Done_Face(face);
        FT_Done_FreeType(lib);
        Log_WriteLine("Done !", LogType::Info, true);

        return new Font(chars, tex, allW, allH, adv, f_h, size);
    }
    TextBlock Font::RenderTextBlock(std::u32string text){
        std::vector<float> verts;
        float xxpos = 0.0f, lineOffset = 0.0f;
        size_t indd = 0;
        for(auto _ch : text){
            auto ch = fc_coll[_ch];
            if (ch.rect.width == 0 || ch.rect.height == 0){
                xxpos += ch.advance;
                continue;
            }
            if (ch.ch == U'\n'){
                lineOffset -= Height();
                xxpos = 0.0f;
                continue;
            }
            drawing::Rectangle rec;
            rec.x = ch.rect.x / t_width;
            rec.y = 0;
            rec.width = ch.rect.width / t_width;
            rec.height = 1;
            float ypos = lineOffset + (0 - (ch.rect.height - ch.bearing.y)) - 1;   
            float xpos = xxpos + ch.bearing.x;
            verts.push_back(xpos); verts.push_back(ypos + t_height);                       verts.push_back(rec.x); verts.push_back(rec.y);    verts.push_back(indd); verts.push_back(0.0f);
            verts.push_back(xpos); verts.push_back(ypos);                                  verts.push_back(rec.x); verts.push_back(rec.Bottom());    verts.push_back(indd); verts.push_back(0.0f);
            verts.push_back(xpos + ch.rect.width); verts.push_back(ypos);                  verts.push_back(rec.Right()); verts.push_back(rec.Bottom());    verts.push_back(indd); verts.push_back(0.0f);
            verts.push_back(xpos); verts.push_back(ypos+ t_height);                        verts.push_back(rec.x); verts.push_back(rec.y);    verts.push_back(indd); verts.push_back(0.0f);
            verts.push_back(xpos + ch.rect.width); verts.push_back(ypos);                  verts.push_back(rec.Right()); verts.push_back(rec.Bottom());    verts.push_back(indd); verts.push_back(0.0f);
            verts.push_back(xpos + ch.rect.width); verts.push_back(ypos + t_height);       verts.push_back(rec.Right()); verts.push_back(rec.y);    verts.push_back(indd); verts.push_back(0.0f);
            xxpos += ch.rect.width + ch.bearing.x;
            indd++;
        }
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 4));
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        auto fv = this->MeasureText(text);
        return TextBlock(VAO, VBO, verts.size() / 4, this, Rectangle(0.0f, 0.0f, fv.x, fv.y));
    }
    void Font::BindTextrue(){
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    F_Vector Font::MeasureText(std::u32string text){
        F_Vector vec;
        size_t count = 0;
        for(auto c : text){
            auto ch = fc_coll[c];
            if (ch.valid){
                vec.x += ch.rect.width + ch.bearing.x;
                if (ch.rect.height > vec.y) vec.y = ch.rect.height;
                count++;
            }
        }
        return F_Vector( vec.x, vec.y );
    }
    f_Char* Font::GetCharMetrics(char32_t ch){
        return fc_coll.getPtr(ch);
    }
    TextBlock::TextBlock(uint32_t vao, uint32_t vbo, int count, drawing::Font* font, Rectangle rect){
        if (vao == UINT32_MAX || vbo == UINT32_MAX) {
            valid = false;
            return;
        }
        this->vao = vao;
        this->vbo = vbo;
        this->font = font;
        this->count = count;
        this->valid = true;
        this->rect = rect;
    }
    TextBlock::~TextBlock(){
        if (!valid) return;
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
    void TextBlock::Render(int x, int y, int vpw, int vph, Color color){
        if (!valid) return;
        auto fcs = shaders::FontShader();
        fcs.Use();
        fcs.UniformRGBColor("textColor", color);
        fcs.UniformFloat("vpWidth", vpw);
        fcs.UniformFloat("vpHeight", vph);
        fcs.UniformBool("coloredGlyph", false);
        fcs.UniformFloat("offsetX", x);
        fcs.UniformFloat("offsetY", y);
        fcs.UniformFloat("HLSI", -1);
        fcs.UniformFloat("HLEI", -1);
        fcs.UniformInt("text", 0);
        glActiveTexture(GL_TEXTURE0);
        font->BindTextrue();
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, count);
        //glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    GLString::GLString(Font* fnt){
        this->fnt = fnt;
    }
    GLString::GLString(Font* fnt, std::u32string str){
        for(auto ch : str){
            chars.push_back(fnt->GetCharMetrics(ch));
        }
        this->fnt = fnt;
    }
    size_t GLString::length(){
        return chars.size();
    }
    size_t GLString::width(){
        size_t w = 0;
        for(auto x : chars)
            w += x->rect.width + 1;
        return w;
    }
    void GLString::insert(size_t pos, char32_t ch){
        chars.insert(chars.begin() + pos, fnt->GetCharMetrics(ch));
    }
    void GLString::remove(size_t pos, size_t len){
        chars.erase(chars.begin() + pos, chars.begin() + pos + len);
    }
    f_Char* GLString::operator[](int i){
        if (i >= chars.size()) return new f_Char();
        else return chars[i];
    }
    void GLString::operator+=(GLString r){
        for (size_t i = 0; i < r.length(); i++)
        {
            chars.push_back(r[i]);
        }
    }
    void GLString::operator+=(char32_t r){
        chars.push_back(fnt->GetCharMetrics(r));
    }
    GLString GLString::operator+(char32_t r){
        GLString str(fnt);
        str += *this;
        str += r;
        return str;
    }
    GLString GLString::operator+(GLString r){
        GLString str(fnt);
        str += *this;
        str += r;
        return str;
    }
    std::u32string GLString::u32string(){
        std::u32string str;
        for(auto x : chars)
            str += x->ch;
        return str;
    }
}