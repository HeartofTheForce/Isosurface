#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/gl.h>
#include <Graphics/Programs/Shader.h>

namespace
{
    GLint CheckShader(const GLuint &shaderId, const GLenum &target, const std::string &type, const std::string &message)
    {
        GLint result;
        glGetShaderiv(shaderId, target, &result);
        if (result == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

            GLchar infoLog[maxLength];
            glGetShaderInfoLog(shaderId, maxLength, nullptr, infoLog);

            std::cout << message << std::endl
                      << type << std::endl
                      << infoLog << std::endl;
        }

        return result;
    }

    GLint CheckProgram(const GLuint &programId, const GLenum &target, const std::string &message)
    {
        GLint result;
        glGetProgramiv(programId, target, &result);
        if (result == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

            GLchar infoLog[maxLength];
            glGetProgramInfoLog(programId, maxLength, nullptr, infoLog);

            std::cout << message << std::endl
                      << infoLog << std::endl;
        }

        return result;
    }
} // namespace

GLuint Shader::Compile() const
{
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFile.open(Path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();

        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    GLuint shaderId = glCreateShader(Type);

    const GLchar *code[] = {shaderCode.c_str()};
    const GLint length[] = {(GLint)shaderCode.length()};

    glShaderSource(shaderId, 1, code, length);
    glCompileShader(shaderId);

    if (CheckShader(shaderId, GL_COMPILE_STATUS, Path, "ERROR::SHADER_COMPILATION_ERROR") == GL_FALSE)
    {
        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

GLuint CompileProgram(const Shader shaders[], int shaderCount)
{
    GLuint programId = glCreateProgram();

    if (programId == 0)
    {
        std::cout << "Error creating shader program!" << std::endl;
        return 0;
    }

    GLuint shaderIds[shaderCount];
    for (int i = 0; i < shaderCount; i++)
    {
        shaderIds[i] = shaders[i].Compile();
        glAttachShader(programId, shaderIds[i]);
    }

    glLinkProgram(programId);
    if (CheckProgram(programId, GL_LINK_STATUS, "ERROR::PROGRAM_LINKING_ERROR") == GL_FALSE)
    {
        glDeleteProgram(programId);
        return 0;
    }

    for (int i = 0; i < shaderCount; i++)
    {
        glDetachShader(programId, shaderIds[i]);
        glDeleteShader(shaderIds[i]);
    }

    glValidateProgram(programId);
    if (CheckProgram(programId, GL_VALIDATE_STATUS, "ERROR::PROGRAM_VALIDATE_ERROR") == GL_FALSE)
    {
        glDeleteProgram(programId);
        return 0;
    }

    return programId;
}
