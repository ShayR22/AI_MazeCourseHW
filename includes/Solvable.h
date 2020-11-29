#ifndef __SOLVABLE__
#define __SOLVABLE__

class Solveable {
public:
	virtual void solveIteration() = 0;
	/* FIXME - add in the future single solve */
	// void solve() = 0;
	inline bool getSolved() { return solved; }
protected:
	bool solved;

};

#endif