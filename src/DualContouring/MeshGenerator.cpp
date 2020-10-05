#include <algorithm>
#include <omp.h>
#include <DualContouring/Coordinate.h>
#include <DualContouring/MeshGenerator.h>

#define EDGE_NO_SIGN_CHANGE -1
#define EDGE_BORDER -2

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
#pragma omp parallel for schedule(auto)
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

    int iX = Coordinate::To1D(x, y, z, _edgeMapX.SizeX, _edgeMapX.SizeY);
    int oX = _edgeMapX.IndexOffset[iX];
    if (oX >= 0)
    {
        if (_edgeMapX.Direction[iX] == 1)
        {
            meshCpu.Vertices[oX + 0] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oX + 1] = _cubeVertices[Coordinate::To1D(x, y - 1, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[oX + 2] = _cubeVertices[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[oX + 3] = _cubeVertices[Coordinate::To1D(x, y - 1, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[oX + 4] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oX + 5] = _cubeVertices[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)]; // unique
        }
        else
        {
            meshCpu.Vertices[oX + 5] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oX + 4] = _cubeVertices[Coordinate::To1D(x, y - 1, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[oX + 3] = _cubeVertices[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[oX + 2] = _cubeVertices[Coordinate::To1D(x, y - 1, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[oX + 1] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oX + 0] = _cubeVertices[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)]; // unique
        }
    }

    int iY = Coordinate::To1D(x, y, z, _edgeMapY.SizeX, _edgeMapY.SizeY);
    int oY = _edgeMapY.IndexOffset[iY];
    if (oY >= 0)
    {
        if (_edgeMapY.Direction[iY] == 1)
        {
            meshCpu.Vertices[oY + 0] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oY + 1] = _cubeVertices[Coordinate::To1D(x - 1, y, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[oY + 2] = _cubeVertices[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[oY + 3] = _cubeVertices[Coordinate::To1D(x - 1, y, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[oY + 4] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oY + 5] = _cubeVertices[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)]; // unique
        }
        else
        {
            meshCpu.Vertices[oY + 5] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oY + 4] = _cubeVertices[Coordinate::To1D(x - 1, y, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[oY + 3] = _cubeVertices[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[oY + 2] = _cubeVertices[Coordinate::To1D(x - 1, y, z - 1, _sizeX, _sizeY)];
            meshCpu.Vertices[oY + 1] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oY + 0] = _cubeVertices[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)]; // unique
        }
    }

    int iZ = Coordinate::To1D(x, y, z, _edgeMapZ.SizeX, _edgeMapZ.SizeY);
    int oZ = _edgeMapZ.IndexOffset[iZ];
    if (oZ >= 0)
    {
        if (_edgeMapZ.Direction[iZ] == 1)
        {
            meshCpu.Vertices[oZ + 0] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oZ + 1] = _cubeVertices[Coordinate::To1D(x - 1, y - 1, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oZ + 2] = _cubeVertices[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[oZ + 3] = _cubeVertices[Coordinate::To1D(x - 1, y - 1, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oZ + 4] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oZ + 5] = _cubeVertices[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)]; // unique
        }
        else
        {
            meshCpu.Vertices[oZ + 5] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oZ + 4] = _cubeVertices[Coordinate::To1D(x - 1, y - 1, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oZ + 3] = _cubeVertices[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)]; // unique

            meshCpu.Vertices[oZ + 2] = _cubeVertices[Coordinate::To1D(x - 1, y - 1, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oZ + 1] = _cubeVertices[Coordinate::To1D(x, y, z, _sizeX, _sizeY)];
            meshCpu.Vertices[oZ + 0] = _cubeVertices[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)]; // unique
        }
    }
}

MeshCpu MeshGenerator::GenerateMesh()
{
    std::fill(_cubeCheck.get(), _cubeCheck.get() + _totalSize, false);
    std::fill(_edgeMapX.IndexOffset.get(), _edgeMapX.IndexOffset.get() + _edgeMapX.TotalSize, EDGE_NO_SIGN_CHANGE);
    std::fill(_edgeMapY.IndexOffset.get(), _edgeMapY.IndexOffset.get() + _edgeMapY.TotalSize, EDGE_NO_SIGN_CHANGE);
    std::fill(_edgeMapZ.IndexOffset.get(), _edgeMapZ.IndexOffset.get() + _edgeMapZ.TotalSize, EDGE_NO_SIGN_CHANGE);

    _counter = 0;
#pragma omp parallel
    {
#pragma omp for schedule(auto)
        for (int i = 0; i < _cachedSDF->TotalSize; i++)
        {
            CalculateEdge(i);
        }
#pragma omp for schedule(auto)
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

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(x + 1, y, z, d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = Coordinate::To1D(x, y, z, _edgeMapX.SizeX, _edgeMapX.SizeY);

    _edgeMapX.Intersection[index] = VertexInterp(0, p0, p1, d0, d1);
    _edgeMapX.Direction[index] = NonZeroSign(d1);

    if (y > 0 && z > 0 && y < _sizeY && z < _sizeZ)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapX.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[Coordinate::To1D(x, y, z, _sizeX, _sizeY)] = true;
        _cubeCheck[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)] = true;
        _cubeCheck[Coordinate::To1D(x, y - 1, z - 1, _sizeX, _sizeY)] = true;
        _cubeCheck[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)] = true;
    }
    else
    {
        _edgeMapX.IndexOffset[index] = EDGE_BORDER;
    }
}

void MeshGenerator::CalculateEdgeY(const int &x, const int &y, const int &z, const float &d0, const glm::vec3 &p0)
{
    if (y >= _sizeY)
        return;

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(x, y + 1, z, d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = Coordinate::To1D(x, y, z, _edgeMapY.SizeX, _edgeMapY.SizeY);

    _edgeMapY.Intersection[index] = VertexInterp(0, p0, p1, d0, d1);
    _edgeMapY.Direction[index] = NonZeroSign(d1);

    if (x > 0 && z > 0 && x < _sizeX && z < _sizeZ)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapY.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[Coordinate::To1D(x, y, z, _sizeX, _sizeY)] = true;
        _cubeCheck[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)] = true;
        _cubeCheck[Coordinate::To1D(x - 1, y, z - 1, _sizeX, _sizeY)] = true;
        _cubeCheck[Coordinate::To1D(x, y, z - 1, _sizeX, _sizeY)] = true;
    }
    else
    {
        _edgeMapY.IndexOffset[index] = EDGE_BORDER;
    }
}

void MeshGenerator::CalculateEdgeZ(const int &x, const int &y, const int &z, const float &d0, const glm::vec3 &p0)
{
    if (z >= _sizeZ)
        return;

    float d1;
    glm::vec3 p1;
    _cachedSDF->Get(x, y, z + 1, d1, p1);
    if (NonZeroSign(d0) == NonZeroSign(d1))
        return;

    int index = Coordinate::To1D(x, y, z, _edgeMapZ.SizeX, _edgeMapZ.SizeY);

    _edgeMapZ.Intersection[index] = VertexInterp(0, p0, p1, d0, d1);
    _edgeMapZ.Direction[index] = NonZeroSign(d1);

    if (x > 0 && y > 0 && x < _sizeX && y < _sizeY)
    {
        int indexOffset;
#pragma omp atomic capture
        indexOffset = _counter++;

        _edgeMapZ.IndexOffset[index] = indexOffset * 6;

        _cubeCheck[Coordinate::To1D(x, y, z, _sizeX, _sizeY)] = true;
        _cubeCheck[Coordinate::To1D(x - 1, y, z, _sizeX, _sizeY)] = true;
        _cubeCheck[Coordinate::To1D(x - 1, y - 1, z, _sizeX, _sizeY)] = true;
        _cubeCheck[Coordinate::To1D(x, y - 1, z, _sizeX, _sizeY)] = true;
    }
    else
    {
        _edgeMapZ.IndexOffset[index] = EDGE_BORDER;
    }
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
    int iX = Coordinate::To1D(x, y, z, _edgeMapX.SizeX, _edgeMapX.SizeY);
    if (_edgeMapX.IndexOffset[iX] != EDGE_NO_SIGN_CHANGE)
    {
        sum += _edgeMapX.Intersection[iX];
        cnt++;
    }
}

void MeshGenerator::AggregateEdgeY(const int &x, const int &y, const int &z, glm::vec3 &sum, int &cnt)
{
    int iY = Coordinate::To1D(x, y, z, _edgeMapY.SizeX, _edgeMapY.SizeY);
    if (_edgeMapY.IndexOffset[iY] != EDGE_NO_SIGN_CHANGE)
    {
        sum += _edgeMapY.Intersection[iY];
        cnt++;
    }
}

void MeshGenerator::AggregateEdgeZ(const int &x, const int &y, const int &z, glm::vec3 &sum, int &cnt)
{
    int iZ = Coordinate::To1D(x, y, z, _edgeMapZ.SizeX, _edgeMapZ.SizeY);
    if (_edgeMapZ.IndexOffset[iZ] != EDGE_NO_SIGN_CHANGE)
    {
        sum += _edgeMapZ.Intersection[iZ];
        cnt++;
    }
}
