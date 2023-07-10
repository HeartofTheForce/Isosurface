#include <Scene/Benchmark.h>
#include <Scene/Demo.h>

std::vector<BenchmarkBuilder> BenchmarkBuilders = {
    []() -> Benchmark {
        Benchmark b;

        b.Name = "Box";
        b.Size = 512;
        auto extent = b.Size * 0.4f;
        auto offset = (b.Size - 1) * 0.5f;
        b.Shape = SDF::Translate(
            glm::vec3(offset),
            SDF::Box(glm::vec3(extent))
        );

        return b;
    },
    []() -> Benchmark {
        Benchmark b;

        b.Name = "Sphere";
        b.Size = 256;
        auto extent = b.Size * 0.4f;
        auto offset = (b.Size - 1) * 0.5f;
        b.Shape = SDF::Translate(
            glm::vec3(offset),
            SDF::Sphere(extent)
        );

        return b;
    },
    []() -> Benchmark {
        Benchmark b;

        b.Name = "Noise";
        b.Size = 128;
        b.Shape = SDF::Noise;

        return b;
    },
};

const size_t ITERATIONS = 10;

int main()
{
    // for (size_t i = 0; i < BenchmarkBuilders.size(); i++)
    // {
    //     ExecuteBenchmark(BenchmarkBuilders[i](), ITERATIONS);
    // }

    Demo(BenchmarkBuilders[2]());

    return 0;
}
