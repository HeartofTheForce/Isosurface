#include <Isosurface/CachedSDF.h>
#include <Isosurface/DualContouring/MeshGenerator.h>
#include <Isosurface/MarchingCubes/MeshGenerator.h>
#include <Scene/Benchmark.h>
#include <chrono>
#include <iostream>
#include <memory>

using namespace DualContouring;

void ExecuteBenchmark(Benchmark benchmark, size_t iterations)
{
    std::shared_ptr<CachedSDF> cachedSDF = std::shared_ptr<CachedSDF>(new CachedSDF{glm::uvec3(benchmark.Size)});
    cachedSDF->Measure(glm::mat4(1.0f), benchmark.Shape);
    MeshGenerator meshGenerator = {cachedSDF};

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t j = 0; j < iterations; j++)
    {
        MeshCpu generatedCpu = meshGenerator.GenerateMesh();
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(stop - start).count() * 1000;

    std::cout << "Benchmark: " << benchmark.Name << " " << duration / iterations << "ms" << std::endl;
}