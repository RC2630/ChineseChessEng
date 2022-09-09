#include "general/ansi_codes.h"
#include "general/parseArguments.h"
#include "general/stringUtility.h"
#include "board.h"
#include "command.h"
#include "instruction.h"
#include "general/file.h"

const string CURR_MATCH_FRAMES = "curr/frames.txt";
const string CURR_MATCH_MOVES = "curr/moves.txt";

int main() {

	file::clearFile(CURR_MATCH_FRAMES);
	file::clearFile(CURR_MATCH_MOVES);

	Board board;
	board.printFrameToFile(CURR_MATCH_FRAMES, false);

	while (board.isGeneralAlive(Side::RED) && board.isGeneralAlive(Side::GREEN)) {

		board.display();

		if (board.nextTurn == Side::RED) {
			cout << ANSI_RED << "\nRed's turn: ";
		} else {
			cout << ANSI_GREEN << "\nGreen's turn: ";
		}

		string instruction;
		std::getline(cin >> ws, instruction);
		cout << ANSI_NORMAL;

		if (strUtil::beginsWith(instruction, "/")) {
			Command(instruction).execute(board);
			continue;
		}

		try {
			string moveLog = board.move(Instruction(instruction));
			cout << ANSI_CYAN << "\n" << moveLog << ".\n" << ANSI_NORMAL;
			file::outputStrVecAddTo({moveLog}, CURR_MATCH_MOVES);
			board.printFrameToFile(CURR_MATCH_FRAMES, false);
		} catch (const InvalidInstructionException& e) {
			cout << ANSI_MAGENTA << "\nInstruction syntax is incorrect. Try again!\n" << ANSI_NORMAL;
		} catch (const invalid_argument& e) {
			cout << ANSI_MAGENTA << "\nInstruction represents an illegal move. Try again!\n" << ANSI_NORMAL;
		}

	}

	board.display();

	if (board.isGeneralAlive(Side::GREEN)) {
		cout << ANSI_GREEN << "\nCongratulations! The GREEN side has won this game!\n" << ANSI_NORMAL;
	} else {
		cout << ANSI_RED << "\nCongratulations! The RED side has won this game!\n" << ANSI_NORMAL;
	}

	cout << ANSI_CYAN << "\nTo save the current frame, enter \"frame\"; to save the entire match, enter \"match\"; otherwise, enter \"none\".\n"
		 << "Enter your choice here: ";
	string saveChoice;
	std::getline(cin >> ws, saveChoice);

	if (saveChoice == "frame") {

		cout << "\nEnter a filename to store the current frame: ";
		string filename;
		std::getline(cin >> ws, filename);

		board.printFrameToFile("saves/" + filename, true);
		cout << "\nFrame has been saved. Starting a new match ......\n" << ANSI_NORMAL;

	} else if (saveChoice == "match") {

		cout << "\nEnter a label to store this match's frames and moves: ";
		string label;
		std::getline(cin >> ws, label);

		vector<string> frames;
		file::inputStrVecFrom(frames, CURR_MATCH_FRAMES);
		file::outputVecTo(frames, "saves/" + label + "_frames.txt");

		vector<string> moves;
		file::inputStrVecFrom(moves, CURR_MATCH_MOVES);
		file::outputVecTo(moves, "saves/" + label + "_moves.txt");

		cout << "\nMatch has been saved (frames in \"saves/" << label << "_frames.txt\" and moves in \"saves/" << label << "_moves.txt\"). "
			 << "Starting a new match ......\n" << ANSI_NORMAL;

	} else {

		cout << "\nNothing has been saved. Starting a new match ......\n" << ANSI_NORMAL;

	}

	main();

}