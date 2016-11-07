#ifndef _VECTOR_2I_H_
#define _VECTOR_2I_H_

#include <ostream>


struct Vector2i
{
	int x, y;

	Vector2i();
	Vector2i(int x, int y);
};

std::ostream& operator <<(std::ostream& out, const Vector2i& v);


#endif // _VECTOR_2I_H_