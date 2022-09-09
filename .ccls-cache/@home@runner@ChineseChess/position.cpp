#include <stdexcept>
#include <string>
#include "position.h"

using namespace std;

bool Position::isValidPosition(int col, int row) {
	return 1 <= col && col <= NUM_COLS && 1 <= row && row <= NUM_ROWS;
}

bool Position::isEatenPosition(int col, int row) {
	return col == EATEN && row == EATEN;
}

Position::Position()
: col(EATEN), row(EATEN)
{ }

Position::Position(int col, int row) {
	if (isValidPosition(col, row) || isEatenPosition(col, row)) {
		this->col = col;
		this->row = row;
	} else {
		throw invalid_argument("col and row need to be within bounds of a chinese chess board");
	}
}

void Position::setToEaten() {
	this->row = EATEN;
	this->col = EATEN;
}

bool Position::isValid() const {
	return isValidPosition(this->col, this->row);
}

bool Position::isEaten() const {
	return isEatenPosition(this->col, this->row);
}

bool Position::operator == (Position other) const {
	return this->row == other.row && this->col == other.col;
}

string Position::toString() const {
	return "(" + to_string(this->col) + ", " + to_string(this->row) + ")";
}