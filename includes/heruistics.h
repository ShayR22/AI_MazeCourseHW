#ifndef __HEURISTICS__
#define __HEURISTICS__

#include <math.h>

template <class T>
double manhattan_distance(T* src, T* tgt)
{
	return fabs(src->getX() - tgt->getX()) + fabs(src->getY() - tgt->getY());
}

template <class T>
double diagonal_distance(T* src, T* tgt)
{
	return fmax(fabs(src->getX() - tgt->getX()), fabs(src->getY() - tgt->getY()));
}

template <class T>
double euclidan_distance(T* src, T* tgt)
{
	return sqrt(pow(src->getX() - tgt->getX(), 2) + pow(src->getY() - tgt->getY(),2));
}


#endif // !__HEURISTICS__
