#ifndef __SOLVABLE__
#define __SOLVABLE__

class Solveable {
protected:
	bool solved;
public:
	Solveable() { solved = false; }
	virtual void solveIteration() = 0;
	/* FIXME - add in the future single solve */
	// void solve() = 0;
	inline bool getSolved() { return solved; }
};

#endif