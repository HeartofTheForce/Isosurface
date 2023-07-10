#pragma once
#include <Isosurface/SDF.h>
#include <functional>

struct Benchmark
{
    std::string Name{""};
    int Size{0};
    SDF::SDF Shape{[](glm::vec3 p) -> float { (void)p;return 0; }};
};

typedef std::function<Benchmark()> BenchmarkBuilder;

void ExecuteBenchmark(Benchmark benchmark, size_t iterations);
