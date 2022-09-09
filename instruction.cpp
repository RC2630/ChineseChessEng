#include "general/parseArguments.h"
#include "instruction.h"

InvalidInstructionException::InvalidInstructionException()
: exception()
{ }

InvalidInstructionException::InvalidInstructionException(const string& message)
: exception()
{ }

Instruction::Instruction(const string& inst) {
	vector<string> parts = parse::parseAllArguments(inst);
	if (parts.size() != 5 || parts.at(2) != "->") {
		throw InvalidInstructionException("instruction has wrong number of input arguments, or the middle argument is not an arrow");
	}
	try {
		this->from = Position(stoi(parts.at(0)), stoi(parts.at(1)));
		this->to = Position(stoi(parts.at(3)), stoi(parts.at(4)));
	} catch (const out_of_range& e) {
		throw InvalidInstructionException("input number too big to store in int type");
	} catch (const invalid_argument& e) {
		throw InvalidInstructionException("input is not an integer, or input number is not within bounds of a Chinese Chess board");
	}
}

InstructionType Instruction::getType(const Board& board, int& indexPiece, int& indexTargetPiece) const {

	indexPiece = board.getIndexOfPieceAtPos(this->from);

	// there is no piece to move
	if (indexPiece == -1) {
		return InstructionType::INVALID;
	}

	// the piece there is of the wrong side (colour)
	if (board.pieces.at(indexPiece)->side != board.nextTurn) {
		return InstructionType::INVALID;
	}

	// at this point, we know that there is a correctly-coloured piece at the "from" position,
	// so we just need to check the "to" position using the polymorphic canSomething() methods now

	indexTargetPiece = board.getIndexOfPieceAtPos(this->to);

	// the piece is trying to move to an empty location
	if (indexTargetPiece == -1) {
		return board.pieces.at(indexPiece)->canMoveTo(this->to, board) ? InstructionType::MOVE : InstructionType::INVALID;
	}
	
	// the piece is trying to move to the location of another friendly piece
	if (board.pieces.at(indexTargetPiece)->side == board.pieces.at(indexPiece)->side) {
		return InstructionType::INVALID;
	}

	// the piece is trying to eat an enemy piece and move to its location
	return board.pieces.at(indexPiece)->canEat(*board.pieces.at(indexTargetPiece), board) ? InstructionType::EAT : InstructionType::INVALID;

}