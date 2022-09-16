#include "general/ansi_codes.h"
#include "general/parseArguments.h"
#include "general/file.h"
#include "board.h"

Board::Board() {

	// the red side goes first
	nextTurn = Side::RED;

	// allocate enough space for the board's 32 pieces
	pieces.reserve(NUM_PIECES);

	// add red non-footman pieces
	pieces.push_back(make_unique<Car>(Position(1, 1), Side::RED));
	pieces.push_back(make_unique<Horse>(Position(2, 1), Side::RED));
	pieces.push_back(make_unique<Elephant>(Position(3, 1), Side::RED));
	pieces.push_back(make_unique<Guard>(Position(4, 1), Side::RED));
	pieces.push_back(make_unique<General>(Position(5, 1), Side::RED));
	pieces.push_back(make_unique<Guard>(Position(6, 1), Side::RED));
	pieces.push_back(make_unique<Elephant>(Position(7, 1), Side::RED));
	pieces.push_back(make_unique<Horse>(Position(8, 1), Side::RED));
	pieces.push_back(make_unique<Car>(Position(9, 1), Side::RED));
	pieces.push_back(make_unique<Cannon>(Position(2, 3), Side::RED));
	pieces.push_back(make_unique<Cannon>(Position(8, 3), Side::RED));

	// add green non-footman pieces
	pieces.push_back(make_unique<Car>(Position(1, 10), Side::GREEN));
	pieces.push_back(make_unique<Horse>(Position(2, 10), Side::GREEN));
	pieces.push_back(make_unique<Elephant>(Position(3, 10), Side::GREEN));
	pieces.push_back(make_unique<Guard>(Position(4, 10), Side::GREEN));
	pieces.push_back(make_unique<General>(Position(5, 10), Side::GREEN));
	pieces.push_back(make_unique<Guard>(Position(6, 10), Side::GREEN));
	pieces.push_back(make_unique<Elephant>(Position(7, 10), Side::GREEN));
	pieces.push_back(make_unique<Horse>(Position(8, 10), Side::GREEN));
	pieces.push_back(make_unique<Car>(Position(9, 10), Side::GREEN));
	pieces.push_back(make_unique<Cannon>(Position(2, 8), Side::GREEN));
	pieces.push_back(make_unique<Cannon>(Position(8, 8), Side::GREEN));

	// add red and green footmen
	for (int i = 1; i <= Position::NUM_COLS; i += 2) {
		pieces.push_back(make_unique<Footman>(Position(i, 4), Side::RED));
		pieces.push_back(make_unique<Footman>(Position(i, 7), Side::GREEN));
	}

}

Board::Board(const string& frame) {
	
	string nextTurnInfo = parse::getCommandName(frame, '|');
	vector<string> piecesInfo = parse::parseAllArguments(frame, false, '|');

	this->nextTurn = ((nextTurnInfo == "GREEN") ? Side::GREEN : Side::RED);

	for (const string& pieceInfo : piecesInfo) {
		
		vector<string> parts = parse::parseAllArguments(pieceInfo);
		Position pos(stoi(parts.at(2)), stoi(parts.at(3)));
		Side side = ((parts.at(1) == "G") ? Side::GREEN : Side::RED);
		string type = parts.at(0);

		if (type == "Car") {
			this->pieces.push_back(make_unique<Car>(pos, side));
		} else if (type == "Horse") {
			this->pieces.push_back(make_unique<Horse>(pos, side));
		} else if (type == "Elephant") {
			this->pieces.push_back(make_unique<Elephant>(pos, side));
		} else if (type == "Guard") {
			this->pieces.push_back(make_unique<Guard>(pos, side));
		} else if (type == "General") {
			this->pieces.push_back(make_unique<General>(pos, side));
		} else if (type == "Cannon") {
			this->pieces.push_back(make_unique<Cannon>(pos, side));
		} else if (type == "Footman") {
			this->pieces.push_back(make_unique<Footman>(pos, side));
		}

	}

}

// returns -1 to signify empty pos
int Board::getIndexOfPieceAtPos(Position pos) const {
	for (int i = 0; i < pieces.size(); i++) {
		if (pieces.at(i)->pos == pos) {
			return i;
		}
	}
	return -1;
}

void Board::displayRiver() {

	// top line divider
	for (int x = 0; x <= CELL_WIDTH * Position::NUM_COLS; x++) {
		cout << "-";
	}
	cout << "\n    ";

	// buffer line
	cout << "|";
	for (int x = 1; x < CELL_WIDTH * Position::NUM_COLS; x++) {
		cout << " ";
	}
	cout << "|\n    ";

	// water line
	cout << "| " << ANSI_BLUE;
	for (int x = 2; x <= CELL_WIDTH * Position::NUM_COLS - 2; x++) {
		cout << "~";
	}
	cout << ANSI_NORMAL << " |\n    ";

	// buffer line (again)
	cout << "|";
	for (int x = 1; x < CELL_WIDTH * Position::NUM_COLS; x++) {
		cout << " ";
	}
	cout << "|\n    ";

}

void Board::display(bool displayProtectedness) const {

	// initial newline
	cout << "\n    ";

	// column numbers
	cout << strUtil::spaces((CELL_WIDTH + 1) / 2) << ANSI_YELLOW;
	for (int x = 1; x < Position::NUM_COLS; x++) {
		cout << x << strUtil::spaces(CELL_WIDTH - 1);
	}
	cout << Position::NUM_COLS << ANSI_NORMAL << "\n\n    ";

	// displaying the board row by row, from the top to the bottom
	for (int y = Position::NUM_ROWS; y >= 1; y--) {

		// river
		if (y == Position::NUM_ROWS / 2) {
			displayRiver();
		}
		
		// top line divider
		for (int x = 0; x <= CELL_WIDTH * Position::NUM_COLS; x++) {
			cout << "-";
		}
		cout << "\n    ";

		// buffer line
		for (int x = 0; x < Position::NUM_COLS; x++) {
			cout << "|" << strUtil::spaces(CELL_WIDTH - 1);
		}
		cout << "|\n";

		// piece line
		cout << ((y == 10) ? " " : "  ") << ANSI_YELLOW << y << ANSI_NORMAL << " ";
		for (int x = 1; x <= Position::NUM_COLS; x++) {
			cout << "|" << strUtil::spaces((CELL_WIDTH - Piece::MAX_PIECE_NAME_LENGTH - 1) / 2);
			int pieceIndex = getIndexOfPieceAtPos(Position(x, y));
			if (pieceIndex == -1) {
				cout << strUtil::spaces(Piece::MAX_PIECE_NAME_LENGTH);
			} else {
				pieces.at(pieceIndex)->printChineseName();
			}
			cout << strUtil::spaces((CELL_WIDTH - Piece::MAX_PIECE_NAME_LENGTH - 1) / 2);
		}
		cout << "|" << ((y == 10) ? " " : "  ") << ANSI_YELLOW << y << ANSI_NORMAL << "\n    ";

		// protectedness line
		for (int x = 1; x <= Position::NUM_COLS; x++) {
			if (displayProtectedness) {
				auto [redProt, greenProt] = const_cast<Board*>(this)->getProtectedness(Position(x, y));
				cout << "| " << ANSI_RED << ((redProt != 0) ? to_string(redProt) : " ") << ANSI_NORMAL
					 << strUtil::spaces(CELL_WIDTH - 3 - to_string(redProt).size() - to_string(greenProt).size())
					 << ANSI_GREEN << ((greenProt != 0) ? to_string(greenProt) : " ") << ANSI_NORMAL << " ";
			} else {
				cout << "|" << strUtil::spaces(CELL_WIDTH - 1);
			}
		}

	}

	// final bottom line
	for (int x = 0; x <= CELL_WIDTH * Position::NUM_COLS; x++) {
		cout << "-";
	}
	cout << "\n\n    ";

	// column numbers (again)
	cout << strUtil::spaces((CELL_WIDTH + 1) / 2) << ANSI_YELLOW;
	for (int x = 1; x < Position::NUM_COLS; x++) {
		cout << x << strUtil::spaces(CELL_WIDTH - 1);
	}
	cout << Position::NUM_COLS << ANSI_NORMAL << "\n";

}

string Board::move(Instruction inst, bool checkCanMove) {
	int indexPiece = -2, indexTargetPiece = -2;
	switch (inst.getType(*this, indexPiece, indexTargetPiece)) {
		case InstructionType::INVALID: {
			throw invalid_argument("instruction invalid");
		break; } case InstructionType::MOVE: {
			string moveLog = this->pieces.at(indexPiece)->moveTo(inst.to, *this, checkCanMove);
			if (checkCanMove) { this->nextTurn = (this->nextTurn == Side::RED) ? Side::GREEN : Side::RED; }
			return moveLog;
		break; } case InstructionType::EAT: {
			string eatLog = this->pieces.at(indexPiece)->eat(*this->pieces.at(indexTargetPiece), *this, checkCanMove);
			if (checkCanMove) { this->nextTurn = (this->nextTurn == Side::RED) ? Side::GREEN : Side::RED; }
			return eatLog;
		}
	}
}

bool Board::isGeneralAlive(Side side) const {
	for (int i = 0; i < pieces.size(); i++) {
		if (pieces.at(i)->name == "General" && pieces.at(i)->side == side) {
			return !pieces.at(i)->pos.isEaten();
		}
	}
	throw runtime_error("this line should never be reached");
}

void Board::printFrameToFile(const string& filename, bool overwrite) const {

	// writes out the side of the next turn
	string frame = ((nextTurn == Side::GREEN) ? "GREEN" : "RED");

	// writes out the piece information, one by one
	for (int i = 0; i < pieces.size(); i++) {
		Piece& piece = *pieces.at(i);
		frame += "|" + piece.name + " " + ((piece.side == Side::GREEN) ? "G" : "R") + " " + to_string(piece.pos.col) + " " + to_string(piece.pos.row);
	}

	// writes out the frame to the file
	if (overwrite) {
		file::outputTo(frame, filename);
	} else {
		file::outputStrVecAddTo({frame}, filename);
	}

}

// returns {redProt, greenProt}
pair<int, int> Board::getProtectedness(Position pos) {

	pair<int, int> protectedness = {0, 0};
	bool posCurrentlyEmpty = (this->getIndexOfPieceAtPos(pos) == -1);
	int originalPiecesSize = this->pieces.size();

	for (int i = 0; i < originalPiecesSize; i++) {

		const Piece& piece = *this->pieces.at(i);

		if (piece.pos.isEaten()) {
			continue;
		}

		// add a temporary piece if the space is currently empty
		if (posCurrentlyEmpty) {
			this->pieces.push_back(make_unique<Footman>(pos, Side::RED)); // choice of Footman and Red are arbitrary
		}

		if (piece.canEat(*this->pieces.at(this->getIndexOfPieceAtPos(pos)), *this)) {
			if (piece.side == Side::RED) {
				protectedness.first++;
			} else {
				protectedness.second++;
			}
		}

		// delete the temporary piece
		if (this->pieces.size() > originalPiecesSize) {
			this->pieces.pop_back();
		}

	}

	return protectedness;

}