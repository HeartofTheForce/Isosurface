#pragma once
#include <string>
#include <glad/gl.h>

struct Shader
{
    GLenum Type;
    std::string Path;

    GLuint Compile() const;

    Shader(GLenum type, std::string path) : Type(type),
                                            Path(path) {}
};

GLuint CompileProgram(const Shader shaders[], int shaderCount);
