

#include "common.h"
#include <iostream>
#include <vector>

bool verbose = false;

#define dout if (verbose) cout

#include "board.h"

using namespace std;


auto loadBoard() {
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
	return vector<int>({
		1, 0, 0,  0, 0, 0,  0, 0, 0,
		0, 0, 0,  0, 0, 0,  0, 0, 0,
		0, 0, 0,  0, 0, 0,  0, 0, 0,

		0, 0, 0,  0, 0, 0,  0, 0, 0,
		0, 0, 0,  0, 0, 0,  0, 0, 0,
		0, 0, 0,  0, 0, 0,  0, 0, 0,

		0, 0, 0,  0, 0, 0,  0, 0, 0,
		0, 0, 0,  0, 0, 0,  0, 0, 0,
		0, 0, 0,  0, 0, 0,  0, 0, 9,
	});
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
//	return vector<int>({
//		0, 0, 0,  0, 0, 9,  2, 6, 0,
//		0, 0, 9,  8, 0, 7,  0, 0, 0,
//		7, 0, 0,  0, 0, 0,  0, 9, 8,
//
//		0, 0, 0,  0, 0, 3,  4, 0, 1,
//		0, 1, 0,  0, 8, 0,  0, 2, 0,
//		5, 0, 4,  7, 0, 0,  0, 0, 0,
//
//		6, 8, 0,  0, 0, 0,  0, 0, 2,
//		0, 0, 0,  2, 0, 1,  5, 0, 0,
//		0, 5, 2,  4, 0, 0,  0, 0, 0,
//	});
}

const string helpString = R"_(

Mattias Lasersköld's sudoku solver

arguments:
   -h --help              print this text
   -g --generate          generate a new sudoku board
   -s --solve [standard]  solve a standard sudoku board
   --json                 print the board in json format
)_";


int main(int argc, char **argv) {
	enum {
		Solve,
		Generate,
		Help,
	} mode = Generate;
	string format;
	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
			string arg(argv[i]);
			if (arg == "-g" || arg == "--generate") {
				mode = Generate;
			}
			else if (arg == "-h" || arg == "--help") {
				mode = Help;
			}
			else if (arg == "-s" || arg == "--solve") {
				mode = Solve;
			}
			else if (arg == "--json") {
				format = "json";
			}
		}
	}


	srand(time(0));
	if (mode == Generate) {
		if (format.empty()) {
			cout << "generating sudoku board..." << endl;
		}

		Board board;
		if (format == "json") cout << "{\"solution\": ";
		board.print(format);
		board.generate();
		if (format == "json") cout << ", \"board\": ";
		board.print(format);
		if (format == "json") cout << "} " << endl;

//		board.solve();
//		board.print();
	}
	else if (mode == Help) {
		cout << helpString << endl;
	}
	else if (mode == Solve) {
		cout << "sudoku" << endl << endl;
		Board board(loadBoard());

		board.print(format);
		while(board.autofill()) {
			board.print(format);
		}
		board.print(format);
	}
	return 0;
}
