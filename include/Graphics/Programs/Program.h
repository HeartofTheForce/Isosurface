#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Graphics/Programs/Shader.h>
#include <Graphics/Meshes/MeshGpu.h>
#include <Graphics/Camera.h>

class Program
{
protected:
    GLuint ProgramId;

public:
    virtual void Use() = 0;
    virtual void GenerateVao(MeshGpu &meshGpu) = 0;

    void SetBool(const std::string &name, const bool &value);
    void SetInt(const std::string &name, const int &value);
    void SetFloat(const std::string &name, const float &value);
    void SetFloat3(const std::string &name, const glm::vec3 &value);
    void SetMatrix3(const std::string &name, const glm::mat3 &value);
    void SetMatrix4(const std::string &name, const glm::mat4 &value);

    Program(int programId) : ProgramId(programId){};
    virtual ~Program();
    Program(const Program &) = delete;
    Program &operator=(const Program &) = delete;
};
