#ifndef _VECTOR_2I_H_
#define _VECTOR_2I_H_


struct Vector2i
{
	int x, y;

	Vector2i() :
		x(0), y(0)
	{
	}

	Vector2i(int x, int y) :
		x(x), y(y)
	{
	}
};


#endif // _VECTOR_2I_H_