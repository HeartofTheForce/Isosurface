#pragma once

namespace Coordinate
{
    void To3D(
        int idx,
        const int &sizeX, const int &sizeY,
        int &x, int &y, int &z);

    int To1D(
        const int &x, const int &y, const int &z,
        const int &sizeX, const int &sizeY);
} // namespace Coordinate
