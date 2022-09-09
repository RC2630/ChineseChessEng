#include <iostream>
#include <cstdlib>
#include "general/ansi_codes.h"
#include "general/parseArguments.h"
#include "general/file.h"
#include "command.h"

Command::Command(const string& com)
: command(com)
{ }

void Command::execute(Board& board) const {
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
		replay(board);
	} else if (parse::commandIs(command, "/exit")) {
		exit();
	} else if (parse::commandIs(command, "/restart")) {
		restart(board);
	} else if (parse::commandIs(command, "/skip")) {
		skip(board);
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

void Command::replay(Board& board) const {
	
	string label = parse::parseArgument(command);
	vector<string> frames, moves;

	file::inputStrVecFrom(frames, "saves/" + label + "_frames.txt");
	file::inputStrVecFrom(moves, "saves/" + label + "_moves.txt");

	file::outputVecTo(frames, COM_CURR_MATCH_FRAMES);
	file::outputVecTo(moves, COM_CURR_MATCH_MOVES);

	cout << ANSI_CYAN << "\nMatch loaded successfully. Replay starting now ......\n" << ANSI_NORMAL;

	for (int i = 0; i < moves.size(); i++) {
		board = Board(frames.at(i));
		board.display();
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