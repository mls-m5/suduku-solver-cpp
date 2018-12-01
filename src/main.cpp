

#include "common.h"
#include <iostream>
#include <vector>

bool verbose = false;

#define dout if (verbose) cout

#include "board.h"

using namespace std;


auto loadBoard(string data) {
	if (!data.empty() && data.size() == 81) {
		vector <int> ret;
		for (auto c: data) {
			ret.push_back(c - '0');
		}
		return ret;
	}
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
   --json                 print the board and solution in json format
   --raw                  print the board and solution as numbers
   -d --data 1234...23    input values to be solved
)_";


int main(int argc, char **argv) {
	enum {
		Solve,
		Generate,
		Help,
	} mode = Generate;
	string format;
	std::string data;
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
			else if (arg == "--raw") {
				format = "raw";
			}
			else if (arg == "-d" || arg == "--data") {
				++i;
				if (i >= argc) {
					break;
				}
				data = argv[i];
				if (data.size() != 81) {
					cout << "data should only contain 81 values" << endl;
					throw runtime_error("data should only contain 81 values");
				}
				for (auto c: data) {
					if (c < '0' || c > '9') {
						cout << "data should only be a value between 0 and 9" << endl;
						throw runtime_error("data should only contain numbers");
					}
				}
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
		if (format.empty()) cout << "sudoku" << endl << endl;
		Board board(loadBoard(data));

		if (format.empty()) board.print(format);
		while(board.autofill()) {
			if (format.empty()) board.print(format);
		}
		if (format == "json") cout << "{\"solution\": ";
		board.print(format);
		if (format == "json") cout << "} " << endl;
	}
	return 0;
}
