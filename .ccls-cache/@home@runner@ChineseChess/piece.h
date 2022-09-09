#ifndef PIECE_DOT_H
#define PIECE_DOT_H

#include "general/stringUtility.h"
#include "position.h"

// forward declaration for Board (definition in "board.h" and "board.cpp")
struct Board;

// represents a side in chinese chess
enum struct Side {
	RED,
	GREEN // black does not display well on console, so green is used instead
};

// represents a chinese chess piece
struct Piece {

	static const int MAX_PIECE_NAME_LENGTH = 8; // length of "elephant"

	static bool isWithinPalace(Position pos, Side side);
	static bool onOtherSideOfRiver(Position pos, Side side);

	Position pos;
	string name;
	wchar_t chineseNameRed;
	wchar_t chineseNameGreen;
	Side side;

	Piece(Position pos, Side side, const string& name, wchar_t cNameRed, wchar_t cNameGreen);

	string moveTo(Position newPos, const Board& board);
	string eat(Piece& enemy, const Board& board);
	wchar_t getChineseName() const;
	void printChineseName() const;
	string toString() const;

	virtual bool canMoveTo(Position newPos, const Board& board) const = 0;
	virtual bool canEat(const Piece& enemy, const Board& board) const = 0;

};

// used for the Car and Cannon pieces (because they both move the same way, i.e. in a straight line)
struct StraightLineMover : public Piece {
	StraightLineMover(Position pos, Side side, const string& name, wchar_t cNameRed, wchar_t cNameGreen);
	bool canMoveTo(Position newPos, const Board& board) const override;
};

// ALL STRUCTS FROM THIS POINT ONWARDS REPRESENT CONCRETE CHINESE CHESS PIECES

struct Car : public StraightLineMover {
	Car(Position pos, Side side);
	bool canEat(const Piece& enemy, const Board& board) const override;
};

struct Horse : public Piece {
	Horse(Position pos, Side side);
	bool canMoveTo(Position newPos, const Board& board) const override;
	bool canEat(const Piece& enemy, const Board& board) const override;
};

struct Elephant : public Piece {
	Elephant(Position pos, Side side);
	bool canMoveTo(Position newPos, const Board& board) const override;
	bool canEat(const Piece& enemy, const Board& board) const override;
};

struct Guard : public Piece {
	Guard(Position pos, Side side);
	bool canMoveTo(Position newPos, const Board& board) const override;
	bool canEat(const Piece& enemy, const Board& board) const override;
};

struct General : public Piece {
	General(Position pos, Side side);
	bool canMoveTo(Position newPos, const Board& board) const override;
	bool canEat(const Piece& enemy, const Board& board) const override;
};

struct Cannon : public StraightLineMover {
	Cannon(Position pos, Side side);
	bool canEat(const Piece& enemy, const Board& board) const override;
};

struct Footman : public Piece {
	Footman(Position pos, Side side);
	bool canMoveTo(Position newPos, const Board& board) const override;
	bool canEat(const Piece& enemy, const Board& board) const override;
};

#endif