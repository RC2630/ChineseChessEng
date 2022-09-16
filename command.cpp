#include <iostream>
#include <cstdlib>
#include "general/ansi_codes.h"
#include "general/parseArguments.h"
#include "general/file.h"
#include "command.h"

Command::Command(const string& com)
: command(com)
{ }

void Command::execute(Board& board, bool& displayProtectedness, bool& hasConceded) const {
	if (parse::commandIs(command, "/help")) {
		cout << ANSI_CYAN << "\n" << COMMAND_HELP << ANSI_NORMAL;
	} else if (parse::commandIs(command, "/save") && parse::parseArgument(command, 1) == "frame") {
		saveFrame(board);
	} else if (parse::commandIs(command, "/save") && parse::parseArgument(command, 1) == "match") {
		saveMatch();
	} else if (parse::commandIs(command, "/load")) {
		load(board);
	} else if (parse::commandIs(command, "/undo")) {
		undo(board);
	} else if (parse::commandIs(command, "/eaten")) {
		eaten(board);
	} else if (parse::commandIs(command, "/replay")) {
		replay(board, displayProtectedness);
	} else if (parse::commandIs(command, "/exit")) {
		exit();
	} else if (parse::commandIs(command, "/restart")) {
		restart(board);
	} else if (parse::commandIs(command, "/skip")) {
		skip(board);
	} else if (parse::commandIs(command, "/clear")) {
		clear(board);
	} else if (parse::commandIs(command, "/place")) {
		place(board);
	} else if (parse::commandIs(command, "/remove")) {
		remove(board);
	} else if (parse::commandIs(command, "/force")) {
		forceMove(board);
	} else if (parse::commandIs(command, "/rebase")) {
		rebase(board);
	} else if (parse::commandIs(command, "/showprot")) {
		showProt(displayProtectedness);
	} else if (parse::commandIs(command, "/concede")) {
		concede(board, hasConceded);
	} else if (parse::commandIs(command, "/reload")) {
		reload(board, displayProtectedness, hasConceded);
	} else {
		cout << ANSI_MAGENTA << "\nInvalid command. Try again!\n" << ANSI_NORMAL;
	}
}

void Command::saveFrame(const Board& board) const {
	string filename = "saves/" + parse::parseArgument(command, 2);
	board.printFrameToFile(filename, true);
	cout << ANSI_CYAN << "\nFrame has been saved.\n" << ANSI_NORMAL;
}

void Command::saveMatch() const {

	string label = parse::parseArgument(command, 2);

	vector<string> frames;
	file::inputStrVecFrom(frames, COM_CURR_MATCH_FRAMES);
	file::outputVecTo(frames, "saves/" + label + "_frames.txt");

	vector<string> moves;
	file::inputStrVecFrom(moves, COM_CURR_MATCH_MOVES);
	file::outputVecTo(moves, "saves/" + label + "_moves.txt");

	cout << ANSI_CYAN << "\nMatch has been saved (frames in \"saves/" << label
		 << "_frames.txt\" and moves in \"saves/" << label << "_moves.txt\").\n" << ANSI_NORMAL;

}

void Command::load(Board& board) const {

	string frame;
	file::inputStrFrom(frame, "saves/" + parse::parseArgument(command));
	board = Board(frame);

	file::clearFile(COM_CURR_MATCH_FRAMES);
	file::clearFile(COM_CURR_MATCH_MOVES);
	board.printFrameToFile(COM_CURR_MATCH_FRAMES, false);

	cout << ANSI_CYAN << "\nFrame loaded successfully.\n" << ANSI_NORMAL;

}

void Command::undo(Board& board) const {
	
	vector<string> frames, moves;
	file::inputStrVecFrom(frames, COM_CURR_MATCH_FRAMES);
	file::inputStrVecFrom(moves, COM_CURR_MATCH_MOVES);

	if (frames.size() == 1 && moves.empty()) {
		cout << ANSI_MAGENTA << "\nSorry, but there are no previous frames. You cannot undo anything.\n" << ANSI_NORMAL;
		return;
	}

	moves.pop_back();
	frames.pop_back();

	file::outputVecTo(moves, COM_CURR_MATCH_MOVES);
	file::outputVecTo(frames, COM_CURR_MATCH_FRAMES);

	board = Board(frames.back());
	cout << ANSI_CYAN << "\nThe previous move has been undone.\n" << ANSI_NORMAL;

}

void Command::eaten(const Board& board) const {

	vector<string> redEaten;
  vector<string> greenEaten;

  for (int i = 0; i < board.pieces.size(); i++) {
    if (board.pieces.at(i)->pos.isEaten()) {
      if (board.pieces.at(i)->side == Side::GREEN) {
        greenEaten.push_back(board.pieces.at(i)->name);
      } else {
        redEaten.push_back(board.pieces.at(i)->name);
      }
    }
  }

  cout << ANSI_CYAN << "\nPieces eaten so far:\n" << ANSI_RED;

  if (redEaten.empty()) {
    cout << "(no red pieces eaten)";
  } else {
    for (int i = 0; i < redEaten.size() - 1; i++) {
      cout << redEaten.at(i) << " ";
    }
    cout << redEaten.back();
  }

  cout << "\n" << ANSI_GREEN;

  if (greenEaten.empty()) {
    cout << "(no green pieces eaten)";
  } else {
    for (int i = 0; i < greenEaten.size() - 1; i++) {
      cout << greenEaten.at(i) << " ";
    }
    cout << greenEaten.back();
  }

  cout << "\n" << ANSI_NORMAL;

}

void Command::replay(Board& board, bool displayProtectedness) const {
	
	string label = parse::parseArgument(command, 1);
	vector<string> frames, moves;

	if (label != COM_INTERNAL_USE_CURR) {

		file::inputStrVecFrom(frames, "saves/" + label + "_frames.txt");
		file::inputStrVecFrom(moves, "saves/" + label + "_moves.txt");

		file::outputVecTo(frames, COM_CURR_MATCH_FRAMES);
		file::outputVecTo(moves, COM_CURR_MATCH_MOVES);

		cout << ANSI_CYAN << "\nMatch loaded successfully. Replay starting now ......\n" << ANSI_NORMAL;

	} else {

		file::inputStrVecFrom(frames, COM_CURR_MATCH_FRAMES);
		file::inputStrVecFrom(moves, COM_CURR_MATCH_MOVES);

		if (parse::parseArgument(command, 2) == "restore") {
			cout << ANSI_CYAN << "\nCurrently saved frames and moves restored. Replay starting now ......\n" << ANSI_NORMAL;
		} else if (parse::parseArgument(command, 2) == "reload") {
			cout << ANSI_CYAN << "\nCurrently saved frames and moves reloaded from save files. "
				 << "Note that external save-file manipulation can lead to incorrect/unexpected replay behaviour. "
				 << "Replay starting now ......\n" << ANSI_NORMAL;
		}
		
	}

	for (int i = 0; i < moves.size(); i++) {
		board = Board(frames.at(i));
		board.display(displayProtectedness);
		cout << ((board.nextTurn == Side::RED) ? ANSI_RED : ANSI_GREEN) << "\n"
			 << moves.at(i) << ".\n" << ANSI_NORMAL;
	}

	board = Board(frames.back());

}

void Command::exit() const {
	cout << ANSI_CYAN << "\nThank you for playing Chinese Chess. Have fun!\n\n" << ANSI_NORMAL;
	file::clearFile(COM_CURR_MATCH_FRAMES);
	file::clearFile(COM_CURR_MATCH_MOVES);
	std::exit(EXIT_SUCCESS);
}

void Command::restart(Board& board) const {
	cout << ANSI_CYAN << "\nStarting a new match ......\n" << ANSI_NORMAL;
	file::clearFile(COM_CURR_MATCH_FRAMES);
	file::clearFile(COM_CURR_MATCH_MOVES);
	board = Board();
	board.printFrameToFile(COM_CURR_MATCH_FRAMES, false);
}

void Command::skip(Board& board) const {
	Side originalSide = board.nextTurn;
	board.nextTurn = ((board.nextTurn == Side::GREEN) ? Side::RED : Side::GREEN);
	cout << ANSI_CYAN << "\nThis turn has been skipped and is given to the other side.\n" << ANSI_NORMAL;
	board.printFrameToFile(COM_CURR_MATCH_FRAMES, false);
	file::outputStrVecAddTo({((originalSide == Side::GREEN) ? "Green" : "Red") + string(" skipped a turn")}, COM_CURR_MATCH_MOVES);
}

void Command::clear(Board& board) const {

	const int MIDDLE_COLUMN = (Position::NUM_COLS + 1) / 2, RED_HOME_ROW = 1, GREEN_HOME_ROW = Position::NUM_ROWS;

	for (int i = 0; i < board.pieces.size(); i++) {
		Piece& piece = *board.pieces.at(i);
		if (piece.name != "General") { // most pieces
			piece.pos.setToEaten();
		} else if (piece.side == Side::RED) { // red general
			piece.pos = Position(MIDDLE_COLUMN, RED_HOME_ROW);
		} else { // green general
			piece.pos = Position(MIDDLE_COLUMN, GREEN_HOME_ROW);
		}
	}

	file::clearFile(COM_CURR_MATCH_FRAMES);
	file::clearFile(COM_CURR_MATCH_MOVES);
	board.printFrameToFile(COM_CURR_MATCH_FRAMES, false);

	cout << ANSI_CYAN << "\nEntire board cleared except for the generals.\n" << ANSI_NORMAL;

}

void Command::place(Board& board) const {

	string sideString = parse::parseArgument(this->command, 1);
	Side sideToPlace = ((sideString == "Red") ? Side::RED : Side::GREEN);
	string pieceToPlace = parse::parseArgument(this->command, 2);
	Position pos;

	try {
		pos = Position(parse::parseNumericalArgument(this->command, 3), parse::parseNumericalArgument(this->command, 4));
	} catch (const invalid_argument& e) {
		cout << ANSI_MAGENTA << "\nPosition is out of bounds, "											// Position constructor throws invalid_argument
			 << "or position inputs are not integers, "													// stod function throws invalid_argument
			 << "or position inputs are missing (incorrect number of arguments).\n" << ANSI_NORMAL;		// parse function throws invalid_argument
		return;
	}

	if (board.getIndexOfPieceAtPos(pos) != -1) {
		cout << ANSI_MAGENTA << "\nThere is already a piece at the given position.\n" << ANSI_NORMAL;
		return;
	}

	for (int i = 0; i < board.pieces.size(); i++) {
		Piece& piece = *board.pieces.at(i);
		if (piece.name == pieceToPlace && piece.side == sideToPlace && piece.pos.isEaten()) {

			piece.pos = pos;
			string message = sideString + " " + pieceToPlace + " placed @ " + pos.toString();

			board.printFrameToFile(COM_CURR_MATCH_FRAMES, false);
			file::outputStrVecAddTo({message}, COM_CURR_MATCH_MOVES);

			cout << ANSI_CYAN << "\n" << message << ".\n" << ANSI_NORMAL;
			return;

		}
	}

	cout << ANSI_MAGENTA << "\nThe given piece name is incorrect, or no copies of this piece has been eaten so far in this match.\n" << ANSI_NORMAL;

}

void Command::remove(Board& board) const {

	Position pos;

	try {
		pos = Position(parse::parseNumericalArgument(this->command, 1), parse::parseNumericalArgument(this->command, 2));
	} catch (const invalid_argument& e) {
		cout << ANSI_MAGENTA << "\nPosition is out of bounds, "											// Position constructor throws invalid_argument
			<< "or position inputs are not integers, "													// stod function throws invalid_argument
			<< "or position inputs are missing (incorrect number of arguments).\n" << ANSI_NORMAL;		// parse function throws invalid_argument
		return;
	}

	int pieceToRemoveIndex = board.getIndexOfPieceAtPos(pos);

	if (pieceToRemoveIndex == -1) {
		cout << ANSI_MAGENTA << "\nThere is no piece at the given position to begin with.\n" << ANSI_NORMAL;
		return;
	}

	Piece& pieceToRemove = *board.pieces.at(pieceToRemoveIndex);
	Position pieceToRemoveOgPos = pieceToRemove.pos;
	pieceToRemove.pos.setToEaten();

	string message = string((pieceToRemove.side == Side::RED) ? "Red" : "Green") +
					 " " + pieceToRemove.name + " @ " + pieceToRemoveOgPos.toString() + " was removed";

	board.printFrameToFile(COM_CURR_MATCH_FRAMES, false);
	file::outputStrVecAddTo({message}, COM_CURR_MATCH_MOVES);

	cout << ANSI_CYAN << "\n" << message << ".\n" << ANSI_NORMAL;

}

void Command::forceMove(Board& board) const {

	string instruction = parse::parseArgumentUntilEnd(this->command);
	string log = "FORCE: ";

	try {
		log += board.move(Instruction(instruction, false), false);
	} catch (const InvalidInstructionException& e) {
		cout << ANSI_MAGENTA << "\nForce move instruction syntax is incorrect. Try again!\n" << ANSI_NORMAL;
		return;
	} catch (const invalid_argument& e) {
		cout << ANSI_MAGENTA << "\nYou are attempting to move a piece from an empty position. Try again!\n" << ANSI_NORMAL;
		return;
	}

	board.printFrameToFile(COM_CURR_MATCH_FRAMES, false);
	file::outputStrVecAddTo({log}, COM_CURR_MATCH_MOVES);

	cout << ANSI_CYAN << "\n" << log << ".\n" << ANSI_NORMAL;

}

void Command::rebase(const Board& board) const {
	file::clearFile(COM_CURR_MATCH_FRAMES);
	file::clearFile(COM_CURR_MATCH_MOVES);
	board.printFrameToFile(COM_CURR_MATCH_FRAMES, false);
	cout << ANSI_CYAN << "\nThe current frame has been set as the first frame of this match. "
		 << "All previous frames and moves have been erased.\n" << ANSI_NORMAL;
}

void Command::concede(const Board& board, bool& hasConceded) const {
	hasConceded = true;
	cout << ((board.nextTurn == Side::RED) ? ANSI_GREEN : ANSI_RED)
		 << "\nCongratulations! The " << ((board.nextTurn == Side::RED) ? "GREEN" : "RED")
		 << " side has won this game, through the concession of the "
		 << ((board.nextTurn == Side::RED) ? "RED" : "GREEN") << " side!\n" << ANSI_NORMAL;
}

void Command::showProt(bool& displayProtectedness) const {
	displayProtectedness = parse::parseBooleanArgument(this->command);
	cout << ANSI_CYAN << "\nWhen the board is displayed from now on, each cell's protectedness will now "
		 << (displayProtectedness ? "" : "NOT ") << "be shown.\n" << ANSI_NORMAL;
}

void Command::reload(Board& board, bool ignore1, bool ignore2) const {
	Command("/replay " + COM_INTERNAL_USE_CURR + " reload").execute(board, ignore1, ignore2);
}