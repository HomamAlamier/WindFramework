#include <Core/Drawing/Shaders.h>
#define LOG_CONSOLE
#define LOG_FN "log"
#include <Core/Services/LogService.h>
#ifdef __ANDROID__
#include <ResourceHeaders/GLes_shaders.h>
#else
#include <ResourceHeaders/GL_shaders.h>
#endif
shaders::Shader* def_shaders[3];
bool b_def_shaders = false;
namespace shaders {
        
    bool checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                Log_WriteLine("SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog, LogType::Error, true);
                std::cout << "[ERROR] SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                return false;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                Log_WriteLine("PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog, LogType::Error, true);
                std::cout << "[ERROR] PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                return false;
            }
        }
        return true;
    }
    GLuint compileShader(std::string vertexCode, std::string fragmentCode, std::string shaderName) {
        Log_WriteLine("Compiling Shader : " + shaderName + "...", LogType::Info, true);
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        if (!checkCompileErrors(vertex, "VERTEX")) return UINT32_MAX;
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        if (!checkCompileErrors(fragment, "FRAGMENT")) return UINT32_MAX;
        // shader Program
        unsigned int ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        if (!checkCompileErrors(ID, "PROGRAM")) return UINT32_MAX;
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return ID;
    }
    unsigned int findUniPos(const char* name, unsigned int id){
        return glGetUniformLocation(id, name);
    }
    Shader::Shader(const char* name, const char* vert, const char* frag){
        compiled = (id = compileShader(vert, frag, name)) != UINT32_MAX;
    }
    bool Shader::UniformMat4(const char* name, glm::mat4 value){
        unsigned int loc = 0;
        if ((loc = findUniPos(name, id)) == UINT32_MAX) return false;
        else {
            glUniformMatrix4fv(loc, 1, false, &value[0][0]);
            return true;
        }
    }
    bool Shader::UniformVec3(const char* name, glm::vec3 value){
        unsigned int loc = 0;
        if ((loc = findUniPos(name, id)) == UINT32_MAX) return false;
        else {
            glUniform3f(loc, value.x, value.y, value.z);
            return true;
        }
    }
    bool Shader::UniformInt(const char* name, int value){
        unsigned int loc = 0;
        if ((loc = findUniPos(name, id)) == UINT32_MAX) return false;
        else {
            glUniform1i(loc, value);
            return true;
        }
    }
    bool Shader::UniformBool(const char* name, bool value){
        unsigned int loc = 0;
        if ((loc = findUniPos(name, id)) == UINT32_MAX) return false;
        else {
            glUniform1i(loc, value);
            return true;
        }
    }
    bool Shader::UniformFloat(const char* name, float value){
        unsigned int loc = 0;
        if ((loc = findUniPos(name, id)) == UINT32_MAX) return false;
        else {
            glUniform1f(loc, value);
            return true;
        }
    }
    bool Shader::UniformRGBColor(const char* name, drawing::Color value){
        unsigned int loc = 0;
        if ((loc = findUniPos(name, id)) == UINT32_MAX) return false;
        else {
            glUniform3f(loc, value.R, value.G, value.B);
            return true;
        }
    }
    bool Shader::UniformRGBAColor(const char* name, drawing::Color value){
        unsigned int loc = 0;
        if ((loc = findUniPos(name, id)) == UINT32_MAX) return false;
        else {
            glUniform4f(loc, value.R, value.G, value.B, value.A);
            return true;
        }
    }
    bool Shader::Use(){
        if (!compiled) return false;
        else{
            glUseProgram(id);
            return true;
        }
    }
    bool Shader::Vaild() { return compiled; }
    void InitAll(){
        if (b_def_shaders) return;
        def_shaders[0] = new shaders::Shader("ColorShader", ColorShader_Vertex, ColorShader_Fragment);
        def_shaders[1] = new shaders::Shader("FontShader", FontShader_Vertex, FontShader_Fragment);
        def_shaders[2] = new shaders::Shader("GlyphShader", GlyphShader_Vertex, GlyphShader_Fragment);
        b_def_shaders = true;
    }
    void Force_InitAll(){
        def_shaders[0] = new shaders::Shader("ColorShader", ColorShader_Vertex, ColorShader_Fragment);
        def_shaders[1] = new shaders::Shader("FontShader", FontShader_Vertex, FontShader_Fragment);
        def_shaders[2] = new shaders::Shader("GlyphShader", GlyphShader_Vertex, GlyphShader_Fragment);
        b_def_shaders = true;
    }
    Shader ColorShader() { return *def_shaders[0]; }
    Shader FontShader() { return *def_shaders[1]; }
    Shader GlyphShader() { return *def_shaders[2]; }
}//build