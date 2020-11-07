#include <BFSSolver.h>


using namespace std;



BFSSolver::BFSSolver(Maze& maze) : maze(maze), solved(false) ,cells(maze.getCells())
{
	Cell* start = maze.getStarts()[0];
	start->setVisiting(true);
	visting.push(start);
}

void BFSSolver::checkCellNeighbors(Cell& cell)
{
	int row = cell.getRow();
	int col = cell.getCol();
	
	Cell* neighbors[4];
	neighbors[0] = &cells[row][col - 1]; // left neighbor
	neighbors[1] = &cells[row][col + 1]; // right neighbor
	neighbors[2] = &cells[row - 1][col]; // top neighbor
	neighbors[3] = &cells[row + 1][col]; // buttom neighbor

	for(int i = 0; i < 4; i++) {
		if (neighbors[i]->getVisited() || neighbors[i]->getState() == CellState::wall)
			continue;

		neighbors[i]->setVisiting(true);
		neighbors[i]->setParent(&cell);
		if (*neighbors[i] == maze.getTarget()) {
			restorePath(*neighbors[i]);
			solved = true;
			return;
		}

		visting.push(neighbors[i]);
	}	
}

void BFSSolver::restorePath(Cell& currentCell)
{
	Cell* temp = currentCell.getParent();
	while (temp != nullptr)
	{
		temp->setState(CellState::path);
		temp = temp->getParent();
	}
}

void BFSSolver::solveIteration()
{
	/*
	1. check if stack is empty || solved
	2. empty the visting stack to a temp stack && mark them as visted.
	3. push to visting temp's neighbors cells.
	4. check if one of temp's neighbors cells is the target.
	5. if we found target - > solved = true.
	*/

	if (visting.empty() || solved)
		return;

	stack <Cell*> temp(visting);

	while (!visting.empty()) {
		Cell* cell = visting.top();
		cell->setVisited(true);
		cell->setVisiting(false);
		visting.pop();
	}
	
	while (!temp.empty()) {
		checkCellNeighbors(*temp.top());
		temp.pop();
	}
	

}