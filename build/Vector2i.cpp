#include "Vector2i.h"


Vector2i::Vector2i() :
	x(0), y(0)
{
}

Vector2i::Vector2i(int x, int y) :
	x(x), y(y)
{
}

std::ostream& operator <<(std::ostream& out, const Vector2i& v)
{
	out << "(" << v.x << ", " << v.y << ")";
	return out;
}
