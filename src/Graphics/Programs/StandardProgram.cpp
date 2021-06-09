#include <Graphics/Programs/StandardProgram.h>

namespace
{
const Shader Shaders[] = {
    Shader(GL_VERTEX_SHADER, "assets/shaders/Standard.vert"),
    Shader(GL_FRAGMENT_SHADER, "assets/shaders/Standard.frag"),
};

bool DefineLayout(const GLuint& layoutIndex, const GLint& layoutSize, const GLenum& type, const MeshGpu& meshGpu, const int& vboIndex)
{
    if (meshGpu.VboIds[vboIndex] != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, meshGpu.VboIds[vboIndex]);
        glVertexAttribPointer(layoutIndex, layoutSize, type, GL_FALSE, meshGpu.VboStrides[vboIndex], (void*)0);
        glEnableVertexAttribArray(layoutIndex);

        return true;
    }

    glDisableVertexAttribArray(layoutIndex);
    return false;
}
} // namespace

void StandardProgram::Use()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(ProgramId);
}

StandardProgram::StandardProgram()
    : Program(CompileProgram(Shaders, 2))
{
}

void StandardProgram::GenerateVao(MeshGpu& meshGpu)
{
    assert(meshGpu.VaoId == 0);
    assert(meshGpu.VboIds != nullptr);
    assert(meshGpu.VboStrides != nullptr);

    glGenVertexArrays(1, &meshGpu.VaoId);
    glBindVertexArray(meshGpu.VaoId);

    const int VertexIndex = 0;
    const int NormalIndex = 1;

    DefineLayout(VertexIndex, 3, GL_FLOAT, meshGpu, MeshCpu::VertexVboIndex);

    if (!DefineLayout(NormalIndex, 3, GL_FLOAT, meshGpu, MeshCpu::NormalVboIndex))
    {
        glVertexAttrib3f(NormalIndex, 0.0f, 0.0f, 0.0f);
    }
}

void StandardProgram::Render(Camera& camera, const MeshGpu& meshGpu, const Light& light, const Material& material, Transform targets[], const int& targetCount)
{
    SetMatrix4("projectionMatrix", camera.GetProjectionMatrix());
    SetMatrix4("viewMatrix", camera.GetViewMatrix());
    SetFloat3("viewPosition", camera.Transform.Position());

    SetFloat3("light.position", light.Position);
    SetFloat3("light.ambient", light.Ambient);
    SetFloat3("light.diffuse", light.Diffuse);
    SetFloat3("light.specular", light.Specular);

    Use();
    glBindVertexArray(meshGpu.VaoId);
    for (int i = 0; i < targetCount; i++)
    {
        SetMatrix4("modelMatrix", targets[i].GetMatrix());
        SetMatrix3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(targets[i].GetMatrix()))));

        SetFloat3("material.diffuse", material.Diffuse);
        SetFloat3("material.specular", material.Specular);
        SetFloat("material.shininess", material.Shininess);

        glDrawArrays(GL_TRIANGLES, 0, meshGpu.VertexCount);
    }
}
