#ifndef POSITION_DOT_H
#define POSITION_DOT_H

#include <string>

using namespace std;

// represents a position on a chinese chess board
struct Position {

	static const int NUM_COLS = 9;
	static const int NUM_ROWS = 10;
	static const int EATEN = -1;

	static bool isValidPosition(int col, int row);
	static bool isEatenPosition(int col, int row);

	int col;
	int row;

	Position();
	Position(int col, int row);

	void setToEaten();
	bool isValid() const;
	bool isEaten() const;
	bool operator == (Position other) const;
	string toString() const;

};

#endif