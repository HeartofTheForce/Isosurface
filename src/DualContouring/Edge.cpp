#include <DualContouring/Edge.h>
#include <DualContouring/Coordinate.h>

Edge &EdgeMap::Get(const int &x, const int &y, const int &z) const
{
    int index = Coordinate::To1D(x, y, z, SizeX, SizeY);
    return Edges[index];
}
