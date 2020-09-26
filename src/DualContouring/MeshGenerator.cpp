#include <algorithm>
#include <omp.h>
#include <DualContouring/Coordinate.h>
#include <DualContouring/MeshGenerator.h>

namespace
{
    glm::vec3 VertexInterp(const float &isoLevel, const glm::vec3 &p1, const glm::vec3 &p2, const float &valp1, const float &valp2)
    {
        if (abs(valp2 - valp1) < 0.00001)
            return p1;

        float mu = (isoLevel - valp1) / (valp2 - valp1);
        return p1 + (p2 - p1) * mu;
    }

    float NonZeroSign(float v)
    {
        float output = glm::sign(v);
        if (output == 0)
            return 1;
        else
            return output;
    }
} // namespace

MeshCpu MeshGenerator::BuildMesh(const int &totalVertices)
{
    MeshCpu mesh = {};
    mesh.VertexCount = totalVertices;

    mesh.Vertices = std::unique_ptr<glm::vec3[]>(new glm::vec3[mesh.VertexCount]);
#pragma omp parallel for schedule(static, 32)
    for (int i = 0; i < _totalSize; i++)
    {
        PopulateMesh(i, mesh);
    }

    mesh.CalculateNormals();
    return mesh;
}

void MeshGenerator::PopulateMesh(const int &index, MeshCpu &meshCpu)
{
    int x, y, z;
    Coordinate::To3D(index, _sizeX, _sizeY, x, y, z);

    Edge &edgeX = _edgeMapX.Get(x, y, z);
    if (edgeX.IndexOffset != -1)
    {
        if (edgeX.Direction == 1)
        {
            meshCpu.Vertices[edgeX.IndexOffset + 0] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeX.IndexOffset + 1] = _cubeVertices[Coordinate::To1D(x, y - 1, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeX.IndexOffset + 2] = _cubeVertices[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[edgeX.IndexOffset + 3] = _cubeVertices[Coordinate::To1D(x, y - 1, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeX.IndexOffset + 4] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeX.IndexOffset + 5] = _cubeVertices[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)]; // unique
        }
        else
        {
            meshCpu.Vertices[edgeX.IndexOffset + 5] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeX.IndexOffset + 4] = _cubeVertices[Coordinate::To1D(x, y - 1, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeX.IndexOffset + 3] = _cubeVertices[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[edgeX.IndexOffset + 2] = _cubeVertices[Coordinate::To1D(x, y - 1, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeX.IndexOffset + 1] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeX.IndexOffset + 0] = _cubeVertices[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)]; // unique
        }
    }

    Edge &edgeY = _edgeMapY.Get(x, y, z);
    if (edgeY.IndexOffset != -1)
    {
        if (edgeY.Direction == 1)
        {
            meshCpu.Vertices[edgeY.IndexOffset + 0] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeY.IndexOffset + 1] = _cubeVertices[Coordinate::To1D(x - 1, y, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeY.IndexOffset + 2] = _cubeVertices[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[edgeY.IndexOffset + 3] = _cubeVertices[Coordinate::To1D(x - 1, y, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeY.IndexOffset + 4] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeY.IndexOffset + 5] = _cubeVertices[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)]; // unique
        }
        else
        {
            meshCpu.Vertices[edgeY.IndexOffset + 5] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeY.IndexOffset + 4] = _cubeVertices[Coordinate::To1D(x - 1, y, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeY.IndexOffset + 3] = _cubeVertices[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[edgeY.IndexOffset + 2] = _cubeVertices[Coordinate::To1D(x - 1, y, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeY.IndexOffset + 1] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeY.IndexOffset + 0] = _cubeVertices[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)]; // unique
        }
    }

    Edge &edgeZ = _edgeMapZ.Get(x, y, z);
    if (edgeZ.IndexOffset != -1)
    {
        if (edgeZ.Direction == 1)
        {
            meshCpu.Vertices[edgeZ.IndexOffset + 0] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeZ.IndexOffset + 1] = _cubeVertices[Coordinate::To1D(x - 1, y - 1, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeZ.IndexOffset + 2] = _cubeVertices[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[edgeZ.IndexOffset + 3] = _cubeVertices[Coordinate::To1D(x - 1, y - 1, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeZ.IndexOffset + 4] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeZ.IndexOffset + 5] = _cubeVertices[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)]; // unique
        }
        else
        {
            meshCpu.Vertices[edgeZ.IndexOffset + 5] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeZ.IndexOffset + 4] = _cubeVertices[Coordinate::To1D(x - 1, y - 1, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeZ.IndexOffset + 3] = _cubeVertices[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[edgeZ.IndexOffset + 2] = _cubeVertices[Coordinate::To1D(x - 1, y - 1, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeZ.IndexOffset + 1] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[edgeZ.IndexOffset + 0] = _cubeVertices[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)]; // unique
        }
    }
}

MeshCpu MeshGenerator::GenerateMesh()
{
    std::fill(_cubeCheck.get(), _cubeCheck.get() + _totalSize, false);

    _counter = 0;
#pragma omp parallel
    {
#pragma omp for schedule(static, 32)
        for (int i = 0; i < _cachedSDF->TotalSize; i++)
        {
            CalculateEdge(i);
        }
#pragma omp for schedule(static, 32)
        for (int i = 0; i < _totalSize; i++)
        {
            CalculateVertex(i);
        }
    }

    return BuildMesh(_counter * 6);
}

void MeshGenerator::CalculateEdge(int index)
{
    float d0 = _cachedSDF->CachedDistances[index];
    glm::vec3 p0 = _cachedSDF->CachedPositions[index];

    int x, y, z;
    Coordinate::To3D(index, _cachedSDF->SizeX, _cachedSDF->SizeY, x, y, z);

    CalculateEdgeX(x, y, z, d0, p0);
    CalculateEdgeY(x, y, z, d0, p0);
    CalculateEdgeZ(x, y, z, d0, p0);
}

void MeshGenerator::CalculateEdgeX(const int &x, const int &y, const int &z, const float &d0, const glm::vec3 &p0)
{
    if (x >= _sizeX)
        return;

    int index = Coordinate::To1D(x, y, z, _edgeMapX.SizeX, _edgeMapX.SizeY);
    if (y <= 0 || z <= 0 ||
        y >= _sizeY || z >= _sizeZ)
    {
        _edgeMapX.Edges[index] = {};
        return;
    }

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(x + 1, y, z, d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
    {
        _edgeMapX.Edges[index] = {};
        return;
    }

    _cubeCheck[Coordinate::To1D(x, y, z, _sizeX, _sizeY)] = true;
    _cubeCheck[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)] = true;
    _cubeCheck[Coordinate::To1D(x, y - 1, z - 1, _sizeX, _sizeY)] = true;
    _cubeCheck[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)] = true;

    int indexOffset;
#pragma omp atomic capture
    indexOffset = _counter++;
    _edgeMapX.Edges[index] = {VertexInterp(0, p0, p1, d0, d1), NonZeroSign(d1), indexOffset * 6};
}

void MeshGenerator::CalculateEdgeY(const int &x, const int &y, const int &z, const float &d0, const glm::vec3 &p0)
{
    if (y >= _sizeY)
        return;

    int index = Coordinate::To1D(x, y, z, _edgeMapY.SizeX, _edgeMapY.SizeY);
    if (x <= 0 || z <= 0 ||
        x >= _sizeX || z >= _sizeZ)
    {
        _edgeMapY.Edges[index] = {};
        return;
    }

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(x, y + 1, z, d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
    {
        _edgeMapY.Edges[index] = {};
        return;
    }

    _cubeCheck[Coordinate::To1D(x, y, z, _sizeX, _sizeY)] = true;
    _cubeCheck[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)] = true;
    _cubeCheck[Coordinate::To1D(x - 1, y, z - 1, _sizeX, _sizeY)] = true;
    _cubeCheck[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)] = true;

    int indexOffset;
#pragma omp atomic capture
    indexOffset = _counter++;
    _edgeMapY.Edges[index] = Edge(VertexInterp(0, p0, p1, d0, d1), NonZeroSign(d1), indexOffset * 6);
}

void MeshGenerator::CalculateEdgeZ(const int &x, const int &y, const int &z, const float &d0, const glm::vec3 &p0)
{
    if (z >= _sizeZ)
        return;

    int index = Coordinate::To1D(x, y, z, _edgeMapZ.SizeX, _edgeMapZ.SizeY);
    if (x <= 0 || y <= 0 ||
        x >= _sizeX || y >= _sizeY)
    {
        _edgeMapZ.Edges[index] = {};
        return;
    }

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(x, y, z + 1, d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
    {
        _edgeMapZ.Edges[index] = {};
        return;
    }

    _cubeCheck[Coordinate::To1D(x, y, z, _sizeX, _sizeY)] = true;
    _cubeCheck[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)] = true;
    _cubeCheck[Coordinate::To1D(x - 1, y - 1, z, _sizeX, _sizeY)] = true;
    _cubeCheck[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)] = true;

    int indexOffset;
#pragma omp atomic capture
    indexOffset = _counter++;
    _edgeMapZ.Edges[index] = Edge(VertexInterp(0, p0, p1, d0, d1), NonZeroSign(d1), indexOffset * 6);
}

void MeshGenerator::CalculateVertex(int index)
{
    int x, y, z;
    Coordinate::To3D(index, _sizeX, _sizeY, x, y, z);
    if (!_cubeCheck[index])
        return;

    int cnt = 0;
    glm::vec3 sum = glm::vec3(0.0f);

    AggregateEdgeX(x, y, z, sum, cnt);
    AggregateEdgeX(x, y + 1, z, sum, cnt);
    AggregateEdgeX(x, y + 1, z + 1, sum, cnt);
    AggregateEdgeX(x, y, z + 1, sum, cnt);

    AggregateEdgeY(x, y, z, sum, cnt);
    AggregateEdgeY(x + 1, y, z, sum, cnt);
    AggregateEdgeY(x + 1, y, z + 1, sum, cnt);
    AggregateEdgeY(x, y, z + 1, sum, cnt);

    AggregateEdgeZ(x, y, z, sum, cnt);
    AggregateEdgeZ(x + 1, y, z, sum, cnt);
    AggregateEdgeZ(x + 1, y + 1, z, sum, cnt);
    AggregateEdgeZ(x, y + 1, z, sum, cnt);

    _cubeVertices[index] = sum / (float)cnt;
}

void MeshGenerator::AggregateEdgeX(const int &x, const int &y, const int &z, glm::vec3 &sum, int &cnt)
{
    int edgeIndex = Coordinate::To1D(x, y, z, _edgeMapX.SizeX, _edgeMapX.SizeY);
    Edge &currentEdge = _edgeMapX.Edges[edgeIndex];
    if (currentEdge.IndexOffset != -1)
    {
        sum += currentEdge.Intersection;
        cnt++;
    }
}

void MeshGenerator::AggregateEdgeY(const int &x, const int &y, const int &z, glm::vec3 &sum, int &cnt)
{
    int edgeIndex = Coordinate::To1D(x, y, z, _edgeMapY.SizeX, _edgeMapY.SizeY);
    Edge &currentEdge = _edgeMapY.Edges[edgeIndex];
    if (currentEdge.IndexOffset != -1)
    {
        sum += currentEdge.Intersection;
        cnt++;
    }
}

void MeshGenerator::AggregateEdgeZ(const int &x, const int &y, const int &z, glm::vec3 &sum, int &cnt)
{
    int edgeIndex = Coordinate::To1D(x, y, z, _edgeMapZ.SizeX, _edgeMapZ.SizeY);
    Edge &currentEdge = _edgeMapZ.Edges[edgeIndex];
    if (currentEdge.IndexOffset != -1)
    {
        sum += currentEdge.Intersection;
        cnt++;
    }
}
