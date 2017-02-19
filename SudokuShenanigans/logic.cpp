
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

class timer {
public:
	timer() : start(clock()) {}
	double elapsed() { return (clock() - start) / CLOCKS_PER_SEC; }
	void reset() { start = clock(); }
private:
	double start;
};


struct Cell{
	int row;
	int col;
	int sec;
	vector<int> possibilities;
};

bool notInRow(const vector<vector<int>>& g, int row, int num){
	for (int c = 0; c < 9; ++c){
		if (g[row][c] == num) return false;
	}
	return true;
}

bool notInCol(const vector<vector<int>>& g, int col, int num){
	for (int r = 0; r < 9; ++r){
		if (g[r][col] == num) return false;
	}
	return true;
}

bool notInSec(const vector<vector<int>>& g, int row, int col, int num){
	for (int r = 0; r < 3; ++r){
		for (int c = 0; c <3; ++c){
			if (g[r + row][c + col] == num) return false;
		}
	}
	return true;
}

int findSec(int r, int c){
	if (r == 0 && c == 0) return 0;
	else if (r == 0 && c == 3) return 1;
	else if (r == 0 && c == 6) return 2;
	else if (r == 3 && c == 0) return 3;
	else if (r == 3 && c == 3) return 4;
	else if (r == 3 && c == 6) return 5;
	else if (r == 6 && c == 0) return 6;
	else if (r == 6 && c == 3) return 7;
	else return 8;
}

//Looks at the sudoku Grid and makes a vector of all the empty cells
//Each struct Cell will have the row, column, section/box, and a vector
//of possible values
void evaluateGrid(const vector<vector<int>>& g, vector<Cell>& b){
	for (int r = 0; r < 9; ++r){
		for (int c = 0; c < 9; ++c){ // Loop through the grid
			vector<int> p; //When we find an empty cell, we'll store its possible values here
			if (g[r][c] == 0){ //Unassigned
				Cell emptyCell;
				for (int num = 1; num < 10; ++num){
					if (notInRow(g, r, num) && notInCol(g, c, num) && notInSec(g, r - r % 3, c - c % 3, num)){
						p.push_back(num);
					}
				}
				emptyCell.col = c; emptyCell.row = r; emptyCell.possibilities = p; emptyCell.sec = findSec(r - r % 3, c - c % 3);
				b.push_back(emptyCell);
			}
		}
	}
}

/*Given a number n, at a given cell at index, ind, checks to see
if a cell in the same row could possibly be that number
or if a cell in the same column could be that number
or if a cell in the same section could be that number*/
bool onlyPoss(const vector<Cell>& l, int row, int col, int sec, int ind, int n){
	bool c = true, r = true, s = true;
	for (int i = 0; i < l.size(); ++i) {
		if (i != ind){
			if (l[i].sec == sec || l[i].row == row || l[i].col == col){
				for (int j = 0; j < l[i].possibilities.size(); ++j){
					if (n == l[i].possibilities[j]){
						if (l[i].sec == sec) s = false;
						if (l[i].row == row) r = false;
						if (l[i].col == col) c = false;
					}
				}
			}
		}
	}
	return c || r || s;
}

/*When we assign a number to a cell, we need to update all the vectors of possible values
for every Cell that is not yet assigned a number*/
void update(vector<Cell>& l, int row, int col, int sec, int num){
	for (int i = 0; i < l.size(); ++i){
		if (l[i].row == row || l[i].col == col || l[i].sec == sec){
			for (int j = 0; j < l[i].possibilities.size(); ++j){
				if (l[i].possibilities[j] == num){
					l[i].possibilities.erase(l[i].possibilities.begin() + j);
				}
			}
		}
	}
}

bool mySort(const Cell& l, const Cell& r){
	return l.possibilities.size() < r.possibilities.size();
}

void solveSudoku(vector<vector<int>>& g, vector<Cell>& b){
	bool posFound = false;
	while (!b.empty()){
		sort(b.begin(), b.end(), mySort);
		int r = 0, c = 0, s = 0;
		for (int i = 0; i < b.size(); ++i){
			if (b[i].possibilities.size() == 0){
				b.erase(b.begin() + i);
			}
			else if (b[i].possibilities.size() == 1){ //if only one possible value > assign that value
				int num = b[i].possibilities[0]; r = b[i].row; c = b[i].col; s = b[i].sec;
				g[r][c] = num;
				b.erase(b.begin() + i);
				update(b, r, c, s, num);
			}
			else{
				for (int j = 0; j < b[i].possibilities.size(); ++j){
					int num = b[i].possibilities[j];
					r = b[i].row;
					c = b[i].col;
					s = b[i].sec;
					if (onlyPoss(b, r, c, s, i, num)){
						g[r][c] = num;
						b.erase(b.begin() + i);
						update(b, r, c, s, num);
						posFound = true;
					}
					if (posFound){
						break;
					}
				}
				if (posFound){
					posFound = false;
					break;
				}
			}
		}
	}
}

void print(const vector<vector<int>>& g){
	for (int r = 0; r < 9; ++r){
		for (int c = 0; c < 9; ++c){
			cout << g[r][c] << " ";
		}
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

	vector<Cell> leftOverCells;
	print(grid);
	evaluateGrid(grid, leftOverCells);
	timer t;
	solveSudoku(grid, leftOverCells);
	cout << "Solved puzzle in " << t.elapsed() << endl;
	print(grid);


}

int main(int argc, const char * argv[]) {
	cout << "Easy Puzzle" << endl;
	solvePuzzle("/Users/Dinesh/Desktop/Easy.txt");
	cout << "______________________________" << endl;
	cout << "\n Medium Puzzle " << endl;
	solvePuzzle("/Users/Dinesh/Desktop/Medium.txt");
	cout << "______________________________" << endl;
	cout << "\nHard Puzzle" << endl;
	solvePuzzle("/Users/Dinesh/Desktop/Hard.txt");
	cout << "______________________________" << endl;
	cin.get();

	/*Logic Only can't solve this puzzle. It just runs forever*/
	cout << "\nPuzzle 50" << endl;
	solvePuzzle("/Users/Dinesh/Desktop/puzz50.txt");

	return 0;
}