#include <Graphics/Camera.h>
#include <Graphics/Programs/StandardProgram.h>
#include <Isosurface/CachedSDF.h>
// #include <Isosurface/DualContouring/MeshGenerator.h>
#include <Isosurface/MarchingCubes/MeshGenerator.h>
#include <PerlinNoise.hpp>
#include <Utility/InputHandler.h>
#include <Utility/WindowHandler.h>
#include <chrono>
#include <glm/glm.hpp>

#define FULLSCREEN true

#if FULLSCREEN
#define WIDTH 1920
#define HEIGHT 1080
#else
#define WIDTH 1280
#define HEIGHT 720
#endif

SDF Box(glm::vec3 b)
{
    return [b](glm::vec3 p) {
        glm::vec3 q = glm::vec3(glm::abs(p.x), glm::abs(p.y), glm::abs(p.z)) - b;
        glm::vec3 qMax = glm::vec3(glm::max(q.x, 0.0f), glm::max(q.y, 0.0f), glm::max(q.z, 0.0f));
        return glm::length(qMax) + glm::min(glm::max(q.x, glm::max(q.y, q.z)), 0.0f);
    };
}

SDF Sphere(float r)
{
    return [r](glm::vec3 p) {
        return glm::length(p) - r;
    };
}

SDF Translate(glm::vec3 offset, SDF f)
{
    return [offset, f](glm::vec3 p) {
        p -= offset;
        return f(p);
    };
}

int32_t octave = 2;
float scale = 0.175f;

siv::PerlinNoise perlin(111);
float Noise(glm::vec3 p)
{
    p = p * scale;
    return perlin.normalizedOctaveNoise3D(p.x, p.y, p.z, octave);
}

void Demo()
{
    const int Size = 256;
    const float Extent = Size * 0.4f;
    const float Offset = (Size - 1) * 0.5f;

    // auto shape = Noise;
    // auto shape = Translate(
    //     glm::vec3(Offset, Offset, Offset),
    //     Sphere(Extent));
    auto shape = Translate(
        glm::vec3(Offset, Offset, Offset),
        Box(glm::vec3(Extent, Extent, Extent)));

    GLFWwindow* window = WindowHandler::Instance().CreateWindow(WIDTH, HEIGHT, FULLSCREEN, "Test Title");
    InputHandler inputHandler = {window};

    StandardProgram standardProgram = {};

    Light light = {};
    light.Ambient = glm::vec3(0.2f);
    light.Diffuse = glm::vec3(0.5f);

    Material cubeMaterial = {};
    cubeMaterial.Diffuse = glm::vec3(1.0f);

    std::shared_ptr<CachedSDF> cachedSDF = std::shared_ptr<CachedSDF>(new CachedSDF{glm::ivec3(Size)});
    cachedSDF->Measure(glm::mat4(1.0f), shape);
    MeshGenerator meshGenerator = {cachedSDF};

    Camera camera = {WIDTH, HEIGHT};

    while (inputHandler.BeginFrame())
    {
        glm::vec3 moveDirection = inputHandler.GetMoveDirection();
        glm::vec2 mouseDelta = inputHandler.GetMouseDelta();
        camera.Update(5.0f * inputHandler.DeltaTime, moveDirection, 0.75f, mouseDelta);

        auto start = std::chrono::high_resolution_clock::now();
        MeshCpu generatedCpu = meshGenerator.GenerateMesh();
        MeshGpu generatedGpu = generatedCpu.LoadGpu(standardProgram);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << duration.count() << std::endl;

        Transform generatedTransform = {};

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        light.Position = camera.Transform.Position();
        standardProgram.Render(camera, generatedGpu, light, cubeMaterial, &generatedTransform, 1);

        inputHandler.EndFrame();
    }
}
