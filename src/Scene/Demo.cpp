#include <Graphics/Camera.h>
#include <Graphics/Programs/StandardProgram.h>
#include <Isosurface/CachedSDF.h>
#include <Isosurface/DualContouring/MeshGenerator.h>
#include <Isosurface/MarchingCubes/MeshGenerator.h>
#include <Isosurface/SDF.h>
#include <Scene/Demo.h>
#include <Utility/InputHandler.h>
#include <Utility/WindowHandler.h>
#include <chrono>
#include <glm/glm.hpp>

using namespace DualContouring;

#define FULLSCREEN false

#if FULLSCREEN
#define WIDTH 1920
#define HEIGHT 1080
#else
#define WIDTH 1280
#define HEIGHT 720
#endif

void Demo(Benchmark benchmark)
{
    GLFWwindow* window = WindowHandler::Instance().CreateWindow(WIDTH, HEIGHT, FULLSCREEN, "Test Title");
    InputHandler inputHandler = {window};

    StandardProgram standardProgram = {};

    Light light = {};
    light.Ambient = glm::vec3(0.2f);
    light.Diffuse = glm::vec3(0.5f);

    Material cubeMaterial = {};
    cubeMaterial.Diffuse = glm::vec3(1.0f);

    std::shared_ptr<CachedSDF> cachedSDF = std::shared_ptr<CachedSDF>(new CachedSDF{glm::uvec3(benchmark.Size)});
    cachedSDF->Measure(glm::mat4(1.0f), benchmark.Shape);
    MeshGenerator meshGenerator = {cachedSDF};

    Camera camera = {WIDTH, HEIGHT};

    while (inputHandler.BeginFrame())
    {
        glm::vec3 moveDirection = inputHandler.GetMoveDirection();
        glm::vec2 mouseDelta = inputHandler.GetMouseDelta();
        camera.Update(5.0f * inputHandler.DeltaTime, moveDirection, 2.0f, mouseDelta);

        MeshCpu generatedCpu = meshGenerator.GenerateMesh();
        MeshGpu generatedGpu = generatedCpu.LoadGpu(standardProgram);

        Transform generatedTransform = {};

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        light.Position = camera.Transform.Position();
        standardProgram.Render(camera, generatedGpu, light, cubeMaterial, &generatedTransform, 1);

        inputHandler.EndFrame();
    }
}
