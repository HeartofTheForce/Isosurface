#pragma once
#include <Graphics/Light.h>
#include <Graphics/Material.h>
#include <Graphics/Meshes/MeshCpu.h>
#include <Graphics/Programs/Program.h>
#include <Utility/Transform.h>

class StandardProgram : public Program
{
  public:
    void Use();
    void GenerateVao(MeshGpu& meshGpu);
    void Render(Camera& camera, const MeshGpu& meshGpu, const Light& light, const Material& material, Transform targets[], const int& targetCount);

    StandardProgram();
};
