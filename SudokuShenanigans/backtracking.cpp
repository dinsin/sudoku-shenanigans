
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>

using namespace std;
#define UNASSIGNED 0
#define N 9

class timer {
public:
	timer() : start(clock()) {}
	double elapsed() { return (clock() - start) / CLOCKS_PER_SEC; }
	void reset() { start = clock(); }
private:
	double start;
};

bool FindUnassignedLocation(const vector<vector<int>>& grid, int &row, int &col);
bool isSafe(const vector<vector<int>>& grid, int row, int col, int num);

/* assign values to all unassigned locations for Sudoku solution
*/
bool SolveSudoku(vector<vector<int>>& grid)
{
	int row, col;
	if (!FindUnassignedLocation(grid, row, col))
		return true;
	for (int num = 1; num <= 9; num++)
	{
		if (isSafe(grid, row, col, num))
		{
			grid[row][col] = num;
			if (SolveSudoku(grid))
				return true;
			//cout<< '~' <<endl;
			grid[row][col] = UNASSIGNED;
		}
	}
	return false;
}

/* Searches the grid to find an entry that is still unassigned. */
bool FindUnassignedLocation(const vector<vector<int>>& grid, int &row, int &col)
{
	for (row = 0; row < N; row++)
		for (col = 0; col < N; col++)
			if (grid[row][col] == UNASSIGNED)
				return true;
	return false;
}

/* Returns whether any assigned entry n the specified row matches
the given number. */
bool UsedInRow(const vector<vector<int>>& grid, int row, int num)
{
	for (int col = 0; col < N; col++)
		if (grid[row][col] == num)
			return true;
	return false;
}

/* Returns whether any assigned entry in the specified column matches
the given number. */
bool UsedInCol(const vector<vector<int>>& grid, int col, int num)
{
	for (int row = 0; row < N; row++)
		if (grid[row][col] == num)
			return true;
	return false;
}

/* Returns whether any assigned entry within the specified 3x3 box matches
the given number. */
bool UsedInBox(const vector<vector<int>>& grid, int boxStartRow, int boxStartCol, int num)
{
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++)
			if (grid[row + boxStartRow][col + boxStartCol] == num)
				return true;
	return false;
}

/* Returns whether it will be legal to assign num to the given row,col location.
*/
bool isSafe(const vector<vector<int>>& grid, int row, int col, int num)
{
	return !UsedInRow(grid, row, num) && !UsedInCol(grid, col, num) &&
		!UsedInBox(grid, row - row % 3, col - col % 3, num);
}

/* print grid  */
void printGrid(const vector<vector<int>>& grid)
{
	for (int row = 0; row < N; row++)
	{
		for (int col = 0; col < N; col++)
			cout << grid[row][col] << "  ";
		cout << endl;
	}
}

void solvePuzzle(string fileName){
	ifstream puzzle(fileName);
	int value;
	vector<vector<int>> grid;
	for (int row = 0; row < 9; ++row){
		vector<int> r;
		for (int col = 0; col < 9; ++col){
			puzzle >> value;
			r.push_back(value);
		}
		grid.push_back(r);
	}
	// vector<Cell> leftOverCells;
	printGrid(grid);
	//evaluateGrid(grid, leftOverCells);
	timer t;
	SolveSudoku(grid);
	cout << "Solved puzzle in " << t.elapsed() << endl;
	printGrid(grid);


}
/* Main */
int main()
{
	cout << "Easy Puzzle" << endl;
	solvePuzzle("/Users/Dinesh/Desktop/Easy.txt");
	cout << "______________________________" << endl;
	cout << "\n Medium Puzzle " << endl;
	solvePuzzle("/Users/Dinesh/Desktop/Medium.txt");
	cout << "______________________________" << endl;
	cout << "\nHard Puzzle" << endl;
	solvePuzzle("/Users/Dinesh/Desktop/Hard.txt");
	cout << "______________________________" << endl;
	cout << "\nPuzzle 50" << endl;
	solvePuzzle("/Users/Dinesh/Desktop/puzz50.txt");
	cin.get();

	return 0;
}