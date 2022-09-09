#ifndef COMMAND_DOT_H
#define COMMAND_DOT_H

#include <string>
#include "board.h"

using namespace std;

const string COMMAND_HELP =

	"<x1> <y1> -> <x2> <y2> : move piece from the position (x1, y1) to the position (x2, y2)\n\n"
	
	"/help : displays this help menu\n"
	"/exit : exits the match and terminates the program\n"
	"/skip : skips the current turn, giving the turn to the other side\n"
	"/restart : starts a new match on a fresh board\n"
	"/eaten : inspect which pieces have been eaten so far in this match\n\n"
	
	"/save frame <file> : saves the current frame (state of the board) in a file, to be loaded later\n"
	"/save match <label> : saves the entire progression of the match (all frames and all moves) in 2 files (\"<label>_frames.txt\" and \"<label>_moves.txt\"), to be replayed later\n"
	"/load <file> : loads a frame from a file, so gameplay can continue from that frame\n"
	"/replay <label> : loads an entire progression of a match from 2 files (\"<label>_frames.txt\" and \"<label>_moves.txt\"), so each step can be viewed\n"
	"/undo : reverts to the previous frame\n";

const string COM_CURR_MATCH_FRAMES = "curr/frames.txt";
const string COM_CURR_MATCH_MOVES = "curr/moves.txt";

// represents a special command instruction (ones starting with "/")
struct Command {

	string command;

	Command(const string& com);

	void execute(Board& board) const;

private:

	void saveFrame(const Board& board) const;
	void saveMatch() const;
	void load(Board& board) const;
	void undo(Board& board) const;
	void eaten(const Board& board) const;
	void replay(Board& board) const;
	void exit() const;
	void restart(Board& board) const;
	void skip(Board& board) const;

};

#endif