#pragma once

namespace Coordinate
{
    inline void To3D(
        int idx,
        const int &sizeX, const int &sizeY,
        int &x, int &y, int &z)
    {
        z = idx / (sizeX * sizeY);
        idx -= z * sizeX * sizeY;
        y = idx / sizeX;
        x = idx % sizeX;
    }

    inline int To1D(
        const int &x, const int &y, const int &z,
        const int &sizeX, const int &sizeY)
    {
        return (z * sizeX * sizeY) + (y * sizeX) + x;
    }
} // namespace Coordinate
