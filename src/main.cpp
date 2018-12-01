

#include "common.h"
#include <iostream>
#include <vector>
#include <exception>
#include <algorithm>

using namespace std;


class Cell {
public:
	Cell () {
		resetCalculations();
	}

	operator string() {
		if (value == 0) {
			return "_";
		}
		else if (isAutofillable()) {
			return "x";
		}
		else {
			string str(" ");
			str[0] = '0' + value;
			return str;
		}
	}

	Cell& operator=(int v) {
		value = v;
		return *this;
	}

	void setCoordinates(int x, int y) {
		this->x = x;
		this->y = y;
	}

	void resetCalculations() {
		possibleNumbers.clear();
		for (int i = 0; i < 9; ++i) {
			possibleNumbers.push_back(i + 1);
		}
		uniqueNumber = 0;
	}

	void removeNumber(int number) {
		if (value) {
			return;
		}
		possibleNumbers.erase(remove(
				possibleNumbers.begin(), possibleNumbers.end(), number),
				possibleNumbers.end());
	}

	void addNumber(int number) {
		if (isNumberPossible(number)) {
			return;
		}
		possibleNumbers.push_back(number);
	}

	bool isAutofillable() {
		if (value == 0) {
			if (possibleNumbers.size() == 1) {
				return true;
			}
		}
		return false;
	}

	bool isNumberPossible(int number) {
		if (value) {
			return false;
		}
		return !(find(possibleNumbers.begin(), possibleNumbers.end(), number) == possibleNumbers.end());
	}

	bool autofill() {
		if (value) {
			return true;
		}
		if (possibleNumbers.size() == 1) {
			value = possibleNumbers.front();
			possibleNumbers.clear();
			cout << "putting only possible number " << value << " in cell(" <<
					x +1 << ", " << y + 1 << ")" << endl;
			return true;
		}
		return false;
	}

	void makeUnique(int number) {
		if (value) {
			return;
		}
		uniqueNumber = number;
		possibleNumbers.clear();
		possibleNumbers.push_back(number);
	}

	int value = 0;
	int x = -1;
	int y = -1;

	vector <int> possibleNumbers;
	int uniqueNumber = 0;
};

class Section {
public:
	virtual ~Section() {}

	void setParent(class Board *board) {
		this->board = board;
	}

	void removeNumber(int number) {
		if (number) {
			for (auto cell: cells) {
				cell->removeNumber(number);
			}
		}
	}

	void removeNumbers() {
		for (auto cell: cells) {
			removeNumber(cell->value);
		}
	}

	int possibleNumberPlacements(int n) {
		int count = 0;
		for (auto cell: cells) {
			if (cell->value == n) {
				count = 0; //The number is already placed in a cell
				break;
			}
			else if (cell->isNumberPossible(n)){
				++count;
			}
		}
		return count;
	}

	void checkUniquenes() {
		for (int n = 1; n <= 9; ++n) {
			int count = possibleNumberPlacements(n);
			if (count == 1) {
				//Force a cell to have a number if it is the last cell in that section with that number
				for (auto cell: cells) {
					if (cell->isNumberPossible(n)) {
						cell->makeUnique(n);
					}
					else {
						cell->removeNumber(n);
					}
				}
			}
		}
	}



	std::vector<Cell *> cells;
	class Board *board;
};

class Quadrant: public Section {
public:
	void setQuadrant(vector<Cell> &cells, int x, int y) {
		int qx = x * 3;
		int qy = y * 3;

		for (int cy = qy; cy < qy + 3; ++cy) {
			for (int cx = qx; cx < qx + 3; ++cx) {
				this->cells.push_back(&cells[cx + cy * 9]);
			}
		}
	}

	void checkDoubleRule();
};

class RowColumn: public Section {
public:
	void add(Cell * cell) {
		cells.push_back(cell);
	}

	Cell &operator() (int i) {
		return *cells[i];
	}

};


class Board {
public:
	Board():
		cells(81),
		columns(9),
		rows(9),
		quadrants(9)
	{
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				getCell(i, j).setCoordinates(i, j);
				columns[i].add(&getCell(i, j));
				rows[i].add(&getCell(j, i));
			}
		}

		for (int x = 0; x < 3; ++x) for (int y = 0; y < 3; ++y) {
			quadrant(x, y).setQuadrant(cells, x, y);
		}

		for (auto &column: columns) {
			sections.push_back(&column);
		}
		for (auto &row: rows) {
			sections.push_back(&row);
		}
		for (auto &quadrant: quadrants) {
			sections.push_back(&quadrant);
		}

		for (auto section: sections) {
			section->setParent(this);
		}
	}

	Board(vector<int> values): Board() {
		for (int i = 0; i < values.size(); ++i) {
			cells[i] = values[i];
		}
		recalculate();
	}


	Cell &getCell(int x,int y) {
		if (x >= 9 || y >= 9 || x < 0 || y < 0) {
			throw std::out_of_range("cell index is out of range");
		}
		return cells[x + y * 9];
	}

	void print() {

		cout << "Board status: " << endl;

		for (int y = 0; y < 9; ++y) {
			for (auto cellY = 0; cellY < 3; ++cellY) {
				for (int x = 0; x < 9; ++x) {
					for (auto cellX = 0; cellX < 3; ++cellX) {
						auto n = cellX + cellY * 3 + 1;

						auto &cell = getCell(x, y);
						if (cell.value) {
							if (n == 4) {
								cout << "=";
							}
							else if (n == 5) {
								cout << cell.value;
							}
							else {
								cout << " ";
							}
						}
						else if (cell.isNumberPossible(n)) {
							cout << n;
						}
						else {
							cout << " ";
						}
					}
					if (x % 3 == 2) {
						cout << "|";
					}
					cout << "|";
				}
			cout << endl;
			}
			if (y % 3 == 2) {
				cout << "======================================" << endl;
			}
			else {
				cout << "--------------------------------------" << endl;
			}
		}

		cout << endl << "    ";
		for (int x = 0; x < 9; ++x) {
			cout << x + 1 << " ";
		}
		cout << endl << "   |-----------------|" << endl;
		for (int y = 0; y < 9; ++y) {
			cout << y + 1 << "->|";
			for (int x = 0; x < 9; ++x) {
				cout << (string)getCell(x, y);
				if (x % 3 == 2) {
					cout << "|";
				}
				else {
					cout << " ";
				}
			}
			if (y % 3 == 2) {
				cout << endl << "   |-----+-----+-----|" << endl;
			}
			else {
				cout << endl << "   |     |     |     |" << endl;
			}
		}

		cout << endl;
	}

	Cell &operator() (int x, int y) {
		return getCell(x, y);
	}

	Quadrant &quadrant(int x, int y) {
		if (x >= 9 || y >= 9 || x < 0 || y < 0) {
			throw std::out_of_range("cell index is out of range");
		}
		return quadrants[x + y * 3];
	}

	RowColumn &column(int i) {
		if (i < 0 || i >= 9) {
			throw out_of_range("column index out of range");
		}
		return columns[i];
	}

	RowColumn &row(int i) {
		if (i < 0 || i >= 9) {
			throw out_of_range("row index out of range");
		}
		return rows[i];
	}


	void recalculate() {
		for (auto &cell: cells) {
			cell.resetCalculations();
		}
		for (auto section: sections) {
			section->removeNumbers();
		}
		for (auto section: sections) {
			section->checkUniquenes();
		}
		for (auto &quadrant: quadrants) {
			quadrant.checkDoubleRule();
		}
	}

	bool autofill() {
		recalculate();

		for (auto &cell: cells) {
			cell.autofill();
		}

		recalculate();
		for (auto &cell: cells) {
			if (cell.isAutofillable()) {
				return true;
			}
		}
		return false;
	}

	vector<Cell> cells;
	vector<Quadrant> quadrants;
	vector<RowColumn> columns;
	vector<RowColumn> rows;
	vector<Section *> sections;
};

void Quadrant::checkDoubleRule() {
	for (int n = 1; n <= 9; ++n) {
		int count = possibleNumberPlacements(n);

		if (count == 2) {
			std::vector<Cell*> possibleCells;
			for (auto cell: cells) {
				if (cell->isNumberPossible(n)) {
					possibleCells.push_back(cell);
				}
			}
			if (possibleCells.size() != 2) {
				throw runtime_error ("this should not happend, there should only be two cells in this list");
			}
			if (possibleCells.front()->x == possibleCells.back()->x) {
//				remove the possibility for the other quadrant to use this column
				board->column(possibleCells.front()->x).removeNumber(n);
				possibleCells.front()->addNumber(n);
				possibleCells.back()->addNumber(n);
			}
			else if (possibleCells.front()->y == possibleCells.back()->y) {
				board->row(possibleCells.front()->y).removeNumber(n);
				possibleCells.front()->addNumber(n);
				possibleCells.back()->addNumber(n);
//				cout << "clearing " << n << " from row " << possibleCells.front()->xkk
			}
		}
	}
}

int main(int argc, char **arv) {
//	Board board({
//		0, 6, 0,  0, 0, 0,  8, 1, 0,
//		8, 0, 0,  0, 0, 0,  0, 0, 0,
//		9, 0, 5,  8, 0, 0,  7, 0, 2,
//
//		5, 0, 0,  0, 1, 0,  4, 0, 7,
//		0, 0, 0,  0, 5, 0,  0, 0, 0,
//		2, 0, 7,  0, 3, 0,  0, 0, 8,
//
//		4, 0, 8,  0, 0, 2,  9, 0, 3,
//		0, 0, 0,  0, 0, 0,  0, 0, 1,
//		0, 7, 6,  0, 0, 0,  0, 4, 0,
//	});
//	Board board({
//		1, 0, 0,  0, 0, 0,  0, 0, 0,
//		0, 0, 0,  0, 0, 0,  0, 0, 0,
//		0, 0, 0,  0, 0, 0,  0, 0, 0,
//
//		0, 0, 0,  0, 0, 0,  0, 0, 0,
//		0, 0, 0,  0, 0, 0,  0, 0, 0,
//		0, 0, 0,  0, 0, 0,  0, 0, 0,
//
//		0, 0, 0,  0, 0, 0,  0, 0, 0,
//		0, 0, 0,  0, 0, 0,  0, 0, 0,
//		0, 0, 0,  0, 0, 0,  0, 0, 9,
//	});
//	Board board({ //Lätt
//		0, 9, 3,  0, 0, 6,  0, 0, 5,
//		1, 6, 0,  0, 0, 0,  0, 0, 0,
//		0, 0, 5,  0, 0, 4,  2, 0, 0,
//
//		0, 7, 0,  0, 9, 2,  3, 0, 0,
//		0, 2, 0,  0, 0, 0,  0, 9, 0,
//		0, 0, 4,  6, 5, 0,  0, 1, 0,
//
//		0, 0, 7,  8, 0, 0,  6, 0, 0,
//		0, 0, 0,  0, 0, 0,  0, 3, 1,
//		6, 0, 0,  1, 0, 0,  5, 8, 0,
//   });
	Board board({
		0, 0, 0,  0, 0, 9,  2, 6, 0,
		0, 0, 9,  8, 0, 7,  0, 0, 0,
		7, 0, 0,  0, 0, 0,  0, 9, 8,

		0, 0, 0,  0, 0, 3,  4, 0, 1,
		0, 1, 0,  0, 8, 0,  0, 2, 0,
		5, 0, 4,  7, 0, 0,  0, 0, 0,

		6, 8, 0,  0, 0, 0,  0, 0, 2,
		0, 0, 0,  2, 0, 1,  5, 0, 0,
		0, 5, 2,  4, 0, 0,  0, 0, 0,
	});
	
	board.print();
	while(board.autofill()) {
		board.print();
	}
	board.print();
}
