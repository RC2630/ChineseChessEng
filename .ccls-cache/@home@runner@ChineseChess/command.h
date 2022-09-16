#ifndef COMMAND_DOT_H
#define COMMAND_DOT_H

#include <string>
#include "board.h"

using namespace std;

const string COMMAND_HELP =

	"<x1> <y1> -> <x2> <y2> : moves piece from the position (x1, y1) to the position (x2, y2)\n\n"
	
	"/help : displays this help menu\n"
	"/exit : exits the match and terminates the program\n"
	"/skip : skips the current turn, giving the turn to the other side\n"
	"/restart : starts a new match on a fresh board\n"
	"/eaten : displays all pieces that have been eaten so far in this match\n\n"
	
	"/save frame <file> : saves the current frame (state of the board) in a file, to be loaded later\n"
	"/save match <label> : saves the entire progression of the match (all frames and all moves) in 2 files (\"<label>_frames.txt\" and \"<label>_moves.txt\"), to be replayed later\n"
	"/load <file> : loads a frame from a file, so gameplay can continue from that frame\n"
	"/replay <label> : loads an entire progression of a match from 2 files (\"<label>_frames.txt\" and \"<label>_moves.txt\"), so each step can be viewed\n"
	"/undo : reverts to the previous frame\n"
	"/rebase : sets the current frame as the start of a match (i.e. as the first frame of a match)\n\n"
	
	"/clear : clears the entire field, eating everyone except for the generals, and placing the generals at starting position (also resets all frames)\n"
	"/place <side> <piece> <x> <y> : places a copy of the given piece of the given side (\"Red\"/\"Green\") at (x, y), only if at least one has been eaten already (otherwise it does nothing)\n"
	"/remove <x> <y> : forces the removal of the piece at (x, y), only if a piece previously exists there (otherwise it does nothing)\n"
	"/force <x1> <y1> -> <x2> <y2> : same as regular movement, but disregards piece movement/eating constraints (use at your own risk)\n\n"
	
	"/concede : the side that types this command concedes (surrenders) to its opponent, immediately terminating the current match\n"
	"/showprot <option> : configures whether or not protectedness for each cell is shown when the board is displayed (option is \"true\"/\"false\")\n"
	"/reload : forces a reload of the current in-game frame to be synchronized with the latest saved frame (useful when saved frames are manipulated outside the program)\n";

const string COM_CURR_MATCH_FRAMES = "curr/frames.txt";
const string COM_CURR_MATCH_MOVES = "curr/moves.txt";
const string COM_INTERNAL_USE_CURR = "__CURR__";

// represents a special command instruction (ones starting with "/")
struct Command {

	string command;

	Command(const string& com);

	void execute(Board& board, bool& displayProtectedness, bool& hasConceded) const;

private:

	void saveFrame(const Board& board) const;
	void saveMatch() const;
	void load(Board& board) const;
	void undo(Board& board) const;
	void eaten(const Board& board) const;
	void replay(Board& board, bool displayProtectedness) const;
	void exit() const;
	void restart(Board& board) const;
	void skip(Board& board) const;
	void clear(Board& board) const;
	void place(Board& board) const;
	void remove(Board& board) const;
	void forceMove(Board& board) const;
	void rebase(const Board& board) const;
	void concede(const Board& board, bool& hasConceded) const;
	void showProt(bool& displayProtectedness) const;
	void reload(Board& board, bool ignore1, bool ignore2) const;

};

#endif