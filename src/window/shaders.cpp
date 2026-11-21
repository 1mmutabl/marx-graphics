#include <glad/gl.h>

#include <array>
#include <exception>
#include <iostream>
#include <string>

#include "internal.hpp"

namespace
{
uint make_module(const string &Shader, uint ModuleType)
{
    const char *cShader = Shader.c_str();

    uint ShaderModule = glCreateShader(ModuleType);
    glShaderSource(ShaderModule, 1, &cShader, nullptr);
    glCompileShader(ShaderModule);

    int ShaderStatus;
    glGetShaderiv(ShaderModule, GL_COMPILE_STATUS, &ShaderStatus);

    if (!ShaderStatus)
    {
        char Error[1024];
        glGetShaderInfoLog(ShaderModule, sizeof(Error), nullptr, Error);
        std::cerr << "Shader module compilation error: " << Error << '\n';
        glDeleteShader(ShaderModule);
        std::terminate();
    }

    return ShaderModule;
}
} // namespace

namespace marx::graphics::internal
{
const string VertShader = "#version 330 core\n"
                          "\n"
                          "layout (location=0) in vec2 VertPos;\n"
                          "layout (location=1) in vec4 VertColor;\n"
                          "\n"
                          "uniform vec2 ScreenSize;\n"
                          "uniform vec2 Center;\n"
                          "uniform float Rotation;\n"
                          "\n"
                          "out vec4 FragColor;\n"
                          "out vec2 FragLocalPos;\n"
                          "\n"
                          "void main()\n"
                          "{\n"
                          "  float Angle = radians(Rotation);\n"
                          "  float CosA = cos(Angle);\n"
                          "  float SinA = sin(Angle);\n"
                          "\n"
                          "  mat2 Rotate = mat2(CosA, -SinA, SinA, CosA);\n"
                          "  vec2 LocalPos = VertPos - Center;\n"
                          "  vec2 Pos = Rotate * LocalPos + Center;\n"
                          "\n"
                          "  Pos = (Pos / ScreenSize) * 2.0 - 1.0;\n"
                          "  Pos.y = -Pos.y;\n"
                          "\n"
                          "  gl_Position = vec4(Pos, 0.0, 1.0);\n"
                          "  FragColor = VertColor;\n"
                          "  FragLocalPos = LocalPos;\n"
                          "}";

const string FragShader =
    "#version 330 core\n"
    "\n"
    "in vec4 FragColor;\n"
    "in vec2 FragLocalPos;\n"
    "\n"
    "uniform vec2 Size;\n"
    "uniform float CornerRadius;\n"
    "\n"
    "out vec4 ScreenColor;\n"
    "\n"
    "float RoundedBoxSDF(vec2 p, vec2 b, float r)\n"
    "{\n"
    "  vec2 q = abs(p) - b + r;\n"
    "  return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - r;\n"
    "}\n"
    "\n"
    "void main()\n"
    "{\n"
    "  if (CornerRadius <= 0.0)\n"
    "  {\n"
    "    ScreenColor = FragColor;\n"
    "    return;\n"
    "  }\n"
    "\n"
    "  float Distance = RoundedBoxSDF(\n"
    "      FragLocalPos,\n"
    "      Size * 0.5,\n"
    "      CornerRadius\n"
    "  );\n"
    "\n"
    "  float Alpha = clamp(0.5 - Distance, 0.0, 1.0);\n"
    "\n"
    "  ScreenColor = vec4(FragColor.rgb, FragColor.a * Alpha);\n"
    "}";

const string TextVertShader =
    "#version 330 core\n"
    "\n"
    "layout (location=0) in vec2 VertPos;\n"
    "layout (location=1) in vec2 VertUv;\n"
    "layout (location=2) in vec4 VertColor;\n"
    "\n"
    "uniform vec2 ScreenSize;\n"
    "\n"
    "out vec2 FragUv;\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  vec2 Pos = (VertPos / ScreenSize) * 2.0 - 1.0;\n"
    "  Pos.y = -Pos.y;\n"
    "\n"
    "  gl_Position = vec4(Pos, 0.0, 1.0);\n"
    "  FragUv = VertUv;\n"
    "  FragColor = VertColor;\n"
    "}";

const string TextFragShader =
    "#version 330 core\n"
    "\n"
    "in vec2 FragUv;\n"
    "in vec4 FragColor;\n"
    "\n"
    "uniform sampler2D Atlas;\n"
    "\n"
    "out vec4 ScreenColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  float Coverage = texture(Atlas, FragUv).r;\n"
    "  ScreenColor = vec4(FragColor.rgb, FragColor.a * Coverage);\n"
    "}";

const string SpriteVertShader =
    "#version 330 core\n"
    "\n"
    "layout (location=0) in vec2 VertPos;\n"
    "layout (location=1) in vec2 TexCoord;\n"
    "\n"
    "uniform vec2 ScreenSize;\n"
    "uniform vec2 Center;\n"
    "uniform float Rotation;\n"
    "\n"
    "out vec2 FragTexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  float Angle = radians(Rotation);\n"
    "  float CosA = cos(Angle);\n"
    "  float SinA = sin(Angle);\n"
    "\n"
    "  mat2 Rotate = mat2(CosA, -SinA, SinA, CosA);\n"
    "  vec2 Pos = Rotate * (VertPos - Center) + Center;\n"
    "\n"
    "  Pos = (Pos / ScreenSize) * 2.0 - 1.0;\n"
    "  Pos.y = -Pos.y;\n"
    "\n"
    "  gl_Position = vec4(Pos, 0.0, 1.0);\n"
    "  FragTexCoord = TexCoord;\n"
    "}";

const string SpriteFragShader =
    "#version 330 core\n"
    "\n"
    "in vec2 FragTexCoord;\n"
    "\n"
    "uniform sampler2D Texture;\n"
    "\n"
    "out vec4 ScreenColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  ScreenColor = texture(Texture, FragTexCoord);\n"
    "}";

uint make_shader(const string &Vertex, const string &Fragment)
{
    std::array<uint, 2> Modules = { make_module(Vertex, GL_VERTEX_SHADER),
                                    make_module(Fragment, GL_FRAGMENT_SHADER) };

    uint Shader = glCreateProgram();

    for (uint Module : Modules)
        glAttachShader(Shader, Module);

    glLinkProgram(Shader);

    int ShaderStatus;
    glGetProgramiv(Shader, GL_LINK_STATUS, &ShaderStatus);

    if (!ShaderStatus)
    {
        char Error[1024];
        glGetProgramInfoLog(Shader, sizeof(Error), nullptr, Error);
        std::cerr << "Shader linking error: " << Error << '\n';

        for (uint Module : Modules)
            glDeleteShader(Module);

        glDeleteProgram(Shader);
        std::terminate();
    }

    for (uint Module : Modules)
    {
        glDetachShader(Shader, Module);
        glDeleteShader(Module);
    }

    return Shader;
}
} // namespace marx::graphics::internal
