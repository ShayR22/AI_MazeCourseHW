#ifndef __SOLVABLE__
#define __SOLVABLE__

class Solveable {
protected:
	bool solved;
public:
	Solveable() { solved = false; }
	virtual void solveIteration() = 0;
	void solve()
	{
		while (!solved)
			solveIteration();
	}
	inline bool getSolved() { return solved; }
};

#endif