#pragma once

struct IndexMap
{
    const int SizeX;
    const int SizeY;
    const int SizeZ;
    const int TotalSize;

    IndexMap(
        const int& sizeX,
        const int& sizeY,
        const int& sizeZ)
        : SizeX(sizeX),
          SizeY(sizeY),
          SizeZ(sizeZ),
          TotalSize(SizeX * SizeY * SizeZ)
    {
    }

    inline void To3D(
        int idx,
        int& x, int& y, int& z) const
    {
        z = idx / (SizeX * SizeY);
        idx -= z * SizeX * SizeY;
        y = idx / SizeX;
        x = idx % SizeX;
    }

    inline int To1D(const int& x, const int& y, const int& z) const
    {
        return (z * SizeX * SizeY) + (y * SizeX) + x;
    }
};
