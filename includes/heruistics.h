#ifndef __HEURISTICS__
#define __HEURISTICS__

#include <math.h>

template <class T, class E>
float manhattan_distance(T* src, E* tgt)
{
	return static_cast<float>(fabs(src->getX() - tgt->getX()) + fabs(src->getY() - tgt->getY()));
}

template <class T, class E>
float diagonal_distance(T* src, E* tgt)
{
	return fmax(fabs(src->getX() - tgt->getX()), fabs(src->getY() - tgt->getY()));
}

template <class T, class E>
float euclidan_distance(T* src, E* tgt)
{
	return sqrt(pow(src->getX() - tgt->getX(), 2) + pow(src->getY() - tgt->getY(), 2));
}

template <class E>
float manhattan_distance(E srcX, E srcY, E tgtX, E tgtY)
{
	return fabs(srcX - tgtX) + fabs(srcY - tgtY);
}

template <class E>
float diagonal_distance(E srcX, E srcY, E tgtX, E tgtY)
{
	return fmax(fabs(srcX - tgtX), fabs(srcY - tgtY));
}

template <class E>
float euclidan_distance(E srcX, E srcY, E tgtX, E tgtY)
{
	return static_cast<float>(sqrt(pow(srcX - tgtX, 2) + pow(srcY - tgtY, 2)));
}




#endif // !__HEURISTICS__
