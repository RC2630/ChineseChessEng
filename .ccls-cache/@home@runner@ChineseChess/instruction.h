#ifndef INSTRUCTION_DOT_H
#define INSTRUCTION_DOT_H

#include "position.h"
#include "board.h"

// a custom exception representing an invalid instruction
struct InvalidInstructionException : public exception {
	InvalidInstructionException();
	InvalidInstructionException(const string& message);
};

// represents a type of instruction (move, eat, invalid)
enum struct InstructionType {
	MOVE,
	EAT,
	INVALID
};

// represents an instruction to move a piece on a chinese chess board
struct Instruction {

	Position from;
	Position to;
	bool check;

	Instruction(const string& inst, bool checkCanMove = true);

	InstructionType getType(const Board& board, int& indexPiece, int& indexTargetPiece) const;

};

#endif