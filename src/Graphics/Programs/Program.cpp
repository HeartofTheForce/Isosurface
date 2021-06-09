#include <Graphics/Programs/Program.h>

void Program::SetBool(const std::string& name, const bool& value)
{
    glProgramUniform1i(ProgramId, glGetUniformLocation(ProgramId, name.c_str()), (int)value);
};

void Program::SetInt(const std::string& name, const int& value)
{
    glProgramUniform1i(ProgramId, glGetUniformLocation(ProgramId, name.c_str()), value);
};

void Program::SetFloat(const std::string& name, const float& value)
{
    glProgramUniform1f(ProgramId, glGetUniformLocation(ProgramId, name.c_str()), value);
};

void Program::SetFloat3(const std::string& name, const glm::vec3& value)
{
    glProgramUniform3fv(ProgramId, glGetUniformLocation(ProgramId, name.c_str()), 1, glm::value_ptr(value));
};

void Program::SetMatrix3(const std::string& name, const glm::mat3& value)
{
    glProgramUniformMatrix3fv(ProgramId, glGetUniformLocation(ProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
};

void Program::SetMatrix4(const std::string& name, const glm::mat4& value)
{
    glProgramUniformMatrix4fv(ProgramId, glGetUniformLocation(ProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
};

Program::~Program()
{
    glDeleteProgram(ProgramId);
}
