#ifndef BOARD_DOT_H
#define BOARD_DOT_H

#include <memory>
#include <utility>
#include "general/vectorUtility.h"
#include "piece.h"
#include "instruction.h"

// forward declaration for Instruction (definition in "instruction.h" and "instruction.cpp")
struct Instruction;

// represents a chinese chess board
struct Board {

	static const int NUM_PIECES = 32;
	static const int CELL_WIDTH = 11; // must be an odd number

	static void displayRiver();
	
	Side nextTurn;
	vector<unique_ptr<Piece>> pieces;

	Board();
	Board(const string& frame);

	int getIndexOfPieceAtPos(Position pos) const; // returns -1 to signify empty pos
	void display(bool displayProtectedness) const;
	string move(Instruction inst, bool checkCanMove = true);
	bool isGeneralAlive(Side side) const;
	void printFrameToFile(const string& filename, bool overwrite) const;
	pair<int, int> getProtectedness(Position pos); // returns {redProt, greenProt}

};

#endif