#include <iostream>
#include <cmath>
#include "general/ansi_codes.h"
#include "piece.h"
#include "board.h"

// ************ PIECE ************

bool Piece::isWithinPalace(Position pos, Side side) {
	const int MIDDLE_COL = (Position::NUM_COLS + 1) / 2, BOTTOM_ROW = 1, TOP_ROW = Position::NUM_ROWS;
	if (side == Side::RED) {
		return BOTTOM_ROW <= pos.row && pos.row <= BOTTOM_ROW + 2 && MIDDLE_COL - 1 <= pos.col && pos.col <= MIDDLE_COL + 1;
	} else {
		return TOP_ROW - 2 <= pos.row && pos.row <= TOP_ROW && MIDDLE_COL - 1 <= pos.col && pos.col <= MIDDLE_COL + 1;
	}
}

bool Piece::onOtherSideOfRiver(Position pos, Side side) {
	const int RED_SHORE_ROW = Position::NUM_ROWS / 2, GREEN_SHORE_ROW = Position::NUM_ROWS / 2 + 1;
	if (side == Side::RED) {
		return pos.row > RED_SHORE_ROW;
	} else {
		return pos.row < GREEN_SHORE_ROW;
	}
}

Piece::Piece(Position pos, Side side, const string& name, wchar_t cNameRed, wchar_t cNameGreen)
: pos(pos), name(name), chineseNameRed(cNameRed), chineseNameGreen(cNameGreen), side(side)
{ }

string Piece::moveTo(Position newPos, const Board& board) {
	if (this->canMoveTo(newPos, board)) {
		string moveLog = this->toString() + " moved to " + newPos.toString();
		this->pos = newPos;
		return moveLog;
	} else {
		throw invalid_argument("the new position is not valid for " + this->name);
	}
}

string Piece::eat(Piece& enemy, const Board& board) {
	if (this->canEat(enemy, board)) {
		string eatLog = this->toString() + " ate " + enemy.toString();
		this->pos = enemy.pos;
		enemy.pos.setToEaten();
		return eatLog;
	} else {
		throw invalid_argument(this->name + " cannot eat " + enemy.name + " in this configuration");
	}
}

wchar_t Piece::getChineseName() const {
	return (this->side == Side::RED) ? this->chineseNameRed : this->chineseNameGreen;
}

void Piece::printChineseName() const {

	auto padWithSpaces = [] (const string& s) {
		int needSpaces = MAX_PIECE_NAME_LENGTH - s.size();
		int backSpaces = needSpaces / 2;
		int frontSpaces = needSpaces - backSpaces;
		return strUtil::spaces(frontSpaces) + s + strUtil::spaces(backSpaces);
	};

	// displays colour of side
	cout << ((this->side == Side::RED) ? ANSI_RED : ANSI_GREEN);

	// displays name of piece
	cout << padWithSpaces(this->name);

	// returns colour to default
	cout << ANSI_NORMAL;

}

string Piece::toString() const {
	return ((side == Side::RED) ? "Red" : "Green") + string(" ") + name + " @ " + pos.toString();
}

// ************ STRAIGHT LINE MOVER ************

StraightLineMover::StraightLineMover(Position pos, Side side, const string& name, wchar_t cNameRed, wchar_t cNameGreen)
: Piece(pos, side, name, cNameRed, cNameGreen)
{ }

bool StraightLineMover::canMoveTo(Position newPos, const Board& board) const {

	if (this->pos.col == newPos.col) {

		int smallerRow = min(this->pos.row, newPos.row);
		int largerRow = max(this->pos.row, newPos.row);

		for (int row = smallerRow + 1; row < largerRow; row++) {
			if (board.getIndexOfPieceAtPos(Position(this->pos.col, row)) != -1) {
				return false; // straight line movement is not penetrative!
			}
		}

		return true;

	} else if (this->pos.row == newPos.row) {

		int smallerCol = min(this->pos.col, newPos.col);
		int largerCol = max(this->pos.col, newPos.col);

		for (int col = smallerCol + 1; col < largerCol; col++) {
			if (board.getIndexOfPieceAtPos(Position(col, this->pos.row)) != -1) {
				return false; // straight line movement is not penetrative!
			}
		}

		return true;

	} else {

		return false;

	}

}

// ************ CAR ************

Car::Car(Position pos, Side side)
: StraightLineMover(pos, side, "Car", L'车', L'车')
{ }

bool Car::canEat(const Piece& enemy, const Board& board) const {
	return this->canMoveTo(enemy.pos, board);
}

// ************ HORSE ************

Horse::Horse(Position pos, Side side)
: Piece(pos, side, "Horse", L'马', L'马')
{ }

bool Horse::canMoveTo(Position newPos, const Board& board) const {
	
	int diffRow = newPos.row - this->pos.row;
	int diffCol = newPos.col - this->pos.col;

	if (!((abs(diffRow) == 2 && abs(diffCol) == 1) || (abs(diffRow) == 1 && abs(diffCol) == 2))) {
		return false;
	}

	Position tripPos(this->pos.col + diffCol / 2, this->pos.row + diffRow / 2); // smart way to find the horse tripping position
	return board.getIndexOfPieceAtPos(tripPos) == -1; // check no horse tripping

}

bool Horse::canEat(const Piece& enemy, const Board& board) const {
	return this->canMoveTo(enemy.pos, board);
}

// ************ ELEPHANT ************

Elephant::Elephant(Position pos, Side side)
: Piece(pos, side, "Elephant", L'相', L'象')
{ }

bool Elephant::canMoveTo(Position newPos, const Board& board) const {

	if (Piece::onOtherSideOfRiver(newPos, this->side)) {
		return false; // elephant cannot cross river
	}

	if (abs(newPos.col - this->pos.col) == 2 && abs(newPos.row - this->pos.row) == 2) {
		Position average((this->pos.col + newPos.col) / 2, (this->pos.row + newPos.row) / 2);
		return board.getIndexOfPieceAtPos(average) == -1; // check no elephant tripping
	} else {
		return false;
	}

}

bool Elephant::canEat(const Piece& enemy, const Board& board) const {
	return this->canMoveTo(enemy.pos, board);
}

// ************ GUARD ************

Guard::Guard(Position pos, Side side)
: Piece(pos, side, "Guard", L'士', L'士')
{ }

bool Guard::canMoveTo(Position newPos, const Board& board) const {
	if (Piece::isWithinPalace(newPos, this->side)) {
		return abs(newPos.col - this->pos.col) == 1 && abs(newPos.row - this->pos.row) == 1;
	} else {
		return false;
	}
}

bool Guard::canEat(const Piece& enemy, const Board& board) const {
	return this->canMoveTo(enemy.pos, board);
}

// ************ GENERAL ************

General::General(Position pos, Side side)
: Piece(pos, side, "General", L'帅', L'将')
{ }

bool General::canMoveTo(Position newPos, const Board& board) const {
	if (Piece::isWithinPalace(newPos, this->side)) {
		return (abs(newPos.col - this->pos.col) == 1 && newPos.row == this->pos.row)
			|| (abs(newPos.row - this->pos.row) == 1 && newPos.col == this->pos.col);
	} else {
		return false;
	}
}

bool General::canEat(const Piece& enemy, const Board& board) const {

	// regular case
	if (this->canMoveTo(enemy.pos, board)) {
		return true;
	}

	// duelling generals
	if (enemy.name == "General") {

		if (enemy.pos.col != this->pos.col) {
			return false; // not aligned
		}

		int smallerRow = min(this->pos.row, enemy.pos.row);
		int largerRow = max(this->pos.row, enemy.pos.row);

		for (int row = smallerRow + 1; row < largerRow; row++) {
			if (board.getIndexOfPieceAtPos(Position(this->pos.col, row)) != -1) {
				return false; // there are obstacles between the generals
			}
		}

		return true; // instant victory!

	}
	
	// neither of the above
	return false;

}

// ************ CANNON ************

Cannon::Cannon(Position pos, Side side)
: StraightLineMover(pos, side, "Cannon", L'炮', L'炮')
{ }

bool Cannon::canEat(const Piece& enemy, const Board& board) const {
	
	if (this->pos.col == enemy.pos.col) {

		int smallerRow = min(this->pos.row, enemy.pos.row);
		int largerRow = max(this->pos.row, enemy.pos.row);

		int obstacleCount = 0;
		for (int row = smallerRow + 1; row < largerRow; row++) {
			if (board.getIndexOfPieceAtPos(Position(this->pos.col, row)) != -1) {
				obstacleCount++;
			}
		}

		return obstacleCount == 1; // check for exactly 1 cannon frame

	} else if (this->pos.row == enemy.pos.row) {

		int smallerCol = min(this->pos.col, enemy.pos.col);
		int largerCol = max(this->pos.col, enemy.pos.col);

		int obstacleCount = 0;
		for (int col = smallerCol + 1; col < largerCol; col++) {
			if (board.getIndexOfPieceAtPos(Position(col, this->pos.row)) != -1) {
				obstacleCount++;
			}
		}

		return obstacleCount == 1; // check for exactly 1 cannon frame

	} else {

		return false;

	}

}

// ************ FOOTMAN ************

Footman::Footman(Position pos, Side side)
: Piece(pos, side, "Footman", L'兵', L'卒')
{ }

bool Footman::canMoveTo(Position newPos, const Board& board) const {

	auto isMoveForwardOneStep = [] (Position posOld, Position posNew, Side side) {
		if (side == Side::RED) {
			return posNew.row == posOld.row + 1 && posNew.col == posOld.col;
		} else {
			return posNew.row == posOld.row - 1 && posNew.col == posOld.col;
		}
	};

	if (Piece::onOtherSideOfRiver(this->pos, this->side)) {
		// can move forward or to the side
		return isMoveForwardOneStep(this->pos, newPos, this->side) || (this->pos.row == newPos.row && abs(newPos.col - this->pos.col) == 1);
	} else {
		// can only move forward
		return isMoveForwardOneStep(this->pos, newPos, this->side);
	}

}

bool Footman::canEat(const Piece& enemy, const Board& board) const {
	return this->canMoveTo(enemy.pos, board);
}