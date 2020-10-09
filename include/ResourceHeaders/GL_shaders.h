const char ColorShader_Vertex[] = 
"#version 330 core\r\n"
"#define PAD 0.001f\r\n"
"layout (location = 0) in vec3 aPos;\r\n"
"layout (location = 1) in vec3 aColor;\r\n"
"layout (location = 2) in vec2 attrib;\r\n"
"out vec4 color;\r\n"
"uniform float vpWidth;\r\n"
"uniform float vpHeight;\r\n"
"float _bet_m1_and_1(float value) {return value * 2 - 1;}\r\n"
"void main(){\r\n"
"    vec3 v = vec3(_bet_m1_and_1(aPos.x / vpWidth) + PAD, -_bet_m1_and_1(aPos.y / vpHeight) + PAD, aPos.z);\r\n"
"    gl_Position = vec4(v, 1.0);\r\n"
"    color = vec4(aColor, attrib.x);\r\n"
"}\r";
const char ColorShader_Fragment[] = 
"#version 330 core\r\n"
"in vec4 color;\r\n"
"out vec4 FragColor;\r\n"
"uniform sampler2D img;\r\n"
"uniform int dmode;\r\n"
"void main(){\r\n"
"    if (dmode == 0) FragColor = color;\r\n"
"    else if (dmode == 1) FragColor = texture(img, color.xy);\r\n"
"}";
const char FontShader_Vertex[] = 
"#version 330 core\r\n"
"#define PAD 0.001f\r\n"
"layout(location = 0) in vec4 vertex;\r\n"
"layout(location = 1) in vec2 vertex_attrib;\r\n"
"out vec2 TexCoords;\r\n"
"out vec2 hlat;\r\n"
"uniform float vpWidth;\r\n"
"uniform float vpHeight;\r\n"
"uniform float offsetX;\r\n"
"uniform float offsetY;\r\n"
"uniform float HLSI;\r\n"
"uniform float HLEI;\r\n"
"float _bet_m1_and_1(float value) { return value * 2 - 1; }\r\n"
"void main()\r\n"
"{\r\n"
"    gl_Position = vec4(_bet_m1_and_1((vertex.x + offsetX) / vpWidth) + PAD, _bet_m1_and_1((vertex.y + offsetY) / vpHeight) + PAD, 0.0, 1.0);\r\n"
"    TexCoords = vertex.zw;\r\n"
"    if (vertex_attrib.x >= HLSI && vertex_attrib.x <= HLEI) hlat.x = 1; else hlat.x = 0;\r\n"
"}\r\n"
"\r";
const char FontShader_Fragment[] = 
"#version 330 core\r\n"
"in vec2 TexCoords;\r\n"
"in vec2 hlat;\r\n"
"out vec4 FragColor;\r\n"
"uniform sampler2D text;\r\n"
"uniform vec3 textColor;\r\n"
"uniform bool coloredGlyph;\r\n"
"void main(){\r\n"
"    vec4 color;\r\n"
"    if (!coloredGlyph){\r\n"
"        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\r\n"
"        color = vec4(textColor, 1.0) * sampled;\r\n"
"    }\r\n"
"    else{\r\n"
"        color = texture(text, TexCoords).rgba;\r\n"
"    }\r\n"
"    if (hlat.x == 1){\r\n"
"        FragColor = vec4(color.rgb * vec3(0.0, 1.0, 0.0), 1.0 - color.a);// else FragColor = color;\r\n"
"        if (FragColor.a <= 0.1) FragColor = color;\r\n"
"    }\r\n"
"    else FragColor = color;\r\n"
"    //FragColor = color;\r\n"
"}";
const char GlyphShader_Vertex[] = 
"#version 330 core\r\n"
"layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>\r\n"
"out vec2 TexCoords;\r\n"
"uniform mat4 proj;\r\n"
"void main()\r\n"
"{\r\n"
"    gl_Position = proj * vec4(vertex.xy, 0.0, 1.0);\r\n"
"    TexCoords = vertex.zw;\r\n"
"}";
const char GlyphShader_Fragment[] = 
"#version 330 core\r\n"
"in vec2 TexCoords;\r\n"
"out vec4 color;\r\n"
"uniform sampler2D text;\r\n"
"uniform vec3 textColor;\r\n"
"uniform bool coloredGlyph;\r\n"
"void main(){\r\n"
"    if (!coloredGlyph){\r\n"
"        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\r\n"
"        color = vec4(textColor, 1.0) * sampled;\r\n"
"    }\r\n"
"    else{\r\n"
"        color = texture(text, TexCoords).rgba;\r\n"
"    }\r\n"
"}";
const char ImageShader_Vertex[] = 
"#version 330 core\r\n"
"#define PAD 0.001f\r\n"
"layout (location = 0) in vec2 aPos;\r\n"
"layout (location = 1) in vec2 aTexC;\r\n"
"out vec2 texC;\r\n"
"uniform float vpWidth;\r\n"
"uniform float vpHeight;\r\n"
"float _bet_m1_and_1(float value) {return value * 2 - 1;}\r\n"
"void main(){\r\n"
"    vec3 v = vec3(_bet_m1_and_1(aPos.x / vpWidth) + PAD, -_bet_m1_and_1(aPos.y / vpHeight) + PAD, 0.0f);\r\n"
"    gl_Position = vec4(v, 1.0);\r\n"
"    texC = aTexC;\r\n"
"}\r";
const char ImageShader_Fragment[] = 
"#version 330 core\r\n"
"in vec2 texC;\r\n"
"out vec4 FragColor;\r\n"
"uniform sampler2D tex;\r\n"
"void main(){\r\n"
"    FragColor = texture(tex, texC).rgba;\r\n"
"}\r";
