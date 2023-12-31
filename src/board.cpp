#include "board.h"
#include "move-indicator.h"

Board::Board() { }

void Board::init() {
	squares = {
		{ 	-ROOK, -KNIGHT, 	-BISHOP, -QUEEN, -KING, -BISHOP,  -KNIGHT, -ROOK, 		},
		{ 	-PAWN, -PAWN, 		-PAWN,   -PAWN,  -PAWN, -PAWN,    -PAWN,   -PAWN, 		},
		{ 	EMPTY, EMPTY, 		EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, 		},
		{ 	EMPTY, EMPTY, 		EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, 		},
		{ 	EMPTY, EMPTY, 		EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, 		},
		{ 	EMPTY, EMPTY, 		EMPTY, 	 EMPTY,  EMPTY, EMPTY,    EMPTY,   EMPTY, 		},
		{ 	PAWN,  PAWN, 		PAWN,    PAWN,   PAWN,  PAWN,     PAWN,    PAWN, 		},
		{ 	ROOK,  KNIGHT,  	BISHOP,  QUEEN,  KING,  BISHOP,   KNIGHT,  ROOK, 		}
	};

	if (!boardTexture.loadFromFile("res/board.png")) {
		std::cerr << "Board could not be loaded" << std::endl;
		return;
	}

	boardSprite.setTexture(boardTexture);

	loadPieces();
}

void Board::loadPieces() {
	pieces.resize(0);

	if (!piecesTexture.loadFromFile("res/pieces.png")) {
		std::cerr << "Couldn't load pieces" << std::endl;
		return;
	}

	int i = 0;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			int pieceSquare = squares[y][x];
			if (!pieceSquare) continue;
			
			pieces.resize(pieces.size() + 1);

			int color = getSquareColor(pieceSquare);
			pieceSquare = abs(pieceSquare);

			int size = 64;

			sf::Sprite pieceSprite;

			pieceSprite.setTexture(piecesTexture);
			pieceSprite.setTextureRect(
				sf::IntRect((pieceSquare - 1) * size, color * size, size, size)
			);

			pieceSprite.setPosition(x * size, y * size);

			Piece piece(pieceSquare, color, pieceSprite);
			pieces[i] = piece;

			i++;
		}
	}
}

std::vector<float> Board::toScreenCoords(std::vector<int> boardCoords) {
	return { (float)boardCoords[1] * 64, (float)boardCoords[0] * 64 };
}

std::vector<int> Board::operator[](int at) const {
	return squares[at];
}

int Board::operator[](std::vector<int> at) const {
	return squares[at[0]][at[1]];
}

int Board::operator[](std::string at) const {
	return (*this)[at];
}

std::vector<int>& Board::operator[](int at) {
	return squares[at];
}

int& Board::operator[](std::vector<int> at) {
	return squares[at[0]][at[1]];
}

int& Board::operator[](std::string at) {
	std::string files = "abcdefgh";
	int file = files.find(at[0]);

	std::string ranks = "87654321";
	int rank = ranks.find(at[1]);

	return (*this)[rank][file];
}

int Board::getSquareColor(int square) {
	return square < 0 ? BLACK : (square == EMPTY ? -1 : WHITE);
}

int Board::getSquareColor(std::vector<int> square) {
	return isInsideBoard(square) ? getSquareColor((*this)[square]) : -1;
}

std::vector<int> Board::squareAbove(std::vector<int> square) {
	return { square[0] - 1, square[1] };
}

std::vector<int> Board::squareBelow(std::vector<int> square) {
	return { square[0] + 1, square[1] };
}

std::vector<int> Board::squareAtRight(std::vector<int> square) {
	return { square[0], square[1] + 1 };
}

std::vector<int> Board::squareAtLeft(std::vector<int> square) {
	return { square[0], square[1] - 1 };
}

bool Board::isInsideBoard(std::vector<int> coords) {
	return coords[0] >= 0 && coords[0] < squares.size() && coords[1] >= 0 && coords[1] < squares[0].size();
}

void Board::printMoveHistory() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
	int i = 1;
	for (auto playedMove : playedMoves) {
		if (playedMove[WHITE] != "") {
			std::cout << i << ". " << playedMove[WHITE] << " " << playedMove[BLACK] << std::endl;
		}
		i++;
	}
}

std::vector<std::vector<int>> Board::getValidMoves(Piece piece) {
	std::vector<std::vector<int>> validMoves;
	std::vector<int> lastBoardCoords = piece.getLastBoardCoords();

	auto pushMoveIfItIsInsideBoard = [&](std::vector<int> move) {
		if (isInsideBoard(move)) {
			validMoves.push_back(move);
		}
	};

	auto getKingMoves = [&]() {
		pushMoveIfItIsInsideBoard(squareAbove(lastBoardCoords));
		pushMoveIfItIsInsideBoard(squareAtLeft(squareAbove(lastBoardCoords)));
		pushMoveIfItIsInsideBoard(squareAtRight(squareAbove(lastBoardCoords)));
		pushMoveIfItIsInsideBoard(squareAtLeft(lastBoardCoords));
		pushMoveIfItIsInsideBoard(squareAtRight(lastBoardCoords));
		pushMoveIfItIsInsideBoard(squareBelow(lastBoardCoords));
		pushMoveIfItIsInsideBoard(squareAtLeft(squareBelow(lastBoardCoords)));
		pushMoveIfItIsInsideBoard(squareAtRight(squareBelow(lastBoardCoords)));
	};

	auto getKnightMoves = [&]() {
		pushMoveIfItIsInsideBoard(squareAtLeft(squareAbove(squareAbove(lastBoardCoords))));
		pushMoveIfItIsInsideBoard(squareAtRight(squareAbove(squareAbove(lastBoardCoords))));
		pushMoveIfItIsInsideBoard(squareAtLeft(squareBelow(squareBelow(lastBoardCoords))));
		pushMoveIfItIsInsideBoard(squareAtRight(squareBelow(squareBelow(lastBoardCoords))));
		pushMoveIfItIsInsideBoard(squareAbove(squareAtLeft(squareAtLeft(lastBoardCoords))));
		pushMoveIfItIsInsideBoard(squareBelow(squareAtLeft(squareAtLeft(lastBoardCoords))));
		pushMoveIfItIsInsideBoard(squareAbove(squareAtRight(squareAtRight(lastBoardCoords))));
		pushMoveIfItIsInsideBoard(squareBelow(squareAtRight(squareAtRight(lastBoardCoords))));
	};

	auto getBishopMoves = [&]() {
		std::vector<int> currentMove = squareAbove(squareAtRight(lastBoardCoords));

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);
			currentMove = squareAbove(squareAtRight(currentMove));
		}

		if (getSquareColor(currentMove) != piece.getColor()) {
			if (isInsideBoard(currentMove)) {
				validMoves.push_back(currentMove);
			}
		}

		currentMove = squareAbove(squareAtLeft(lastBoardCoords));

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);
			currentMove = squareAbove(squareAtLeft(currentMove));
		}

		if (getSquareColor(currentMove) != piece.getColor()) {
			if (isInsideBoard(currentMove)) {
				validMoves.push_back(currentMove);
			}
		}

		currentMove = squareBelow(squareAtLeft(lastBoardCoords));

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);
			currentMove = squareBelow(squareAtLeft(currentMove));
		}

		if (getSquareColor(currentMove) != piece.getColor()) {
			if (isInsideBoard(currentMove)) {
				validMoves.push_back(currentMove);
			}
		}

		currentMove = squareBelow(squareAtRight(lastBoardCoords));

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);
			currentMove = squareBelow(squareAtRight(currentMove));
		}

		if (getSquareColor(currentMove) != piece.getColor()) {
			if (isInsideBoard(currentMove)) {
				validMoves.push_back(currentMove);
			}
		}
	};

	auto getRookMoves = [&]() {
		std::vector<int> currentMove = squareAtRight(lastBoardCoords);
		std::cout << currentMove[0] << " " << currentMove[1] << std::endl;

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);	
			currentMove = squareAtRight(currentMove);
		}

		if (getSquareColor(currentMove) != -1 != piece.getColor()) {
			pushMoveIfItIsInsideBoard(currentMove);
		}

		currentMove = squareAtLeft(lastBoardCoords);

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);	
			currentMove = squareAtLeft(currentMove);
		}

		if (getSquareColor(currentMove) != -1 != piece.getColor()) {
			pushMoveIfItIsInsideBoard(currentMove);
		}

		currentMove = squareAbove(lastBoardCoords);

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);
			currentMove = squareAbove(currentMove);
		}

		if (getSquareColor(currentMove) != -1 != piece.getColor()) {
			pushMoveIfItIsInsideBoard(currentMove);
		}

		currentMove = squareBelow(lastBoardCoords);

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);
			currentMove = squareBelow(currentMove);
		}

		if (getSquareColor(currentMove) != -1 != piece.getColor()) {
			pushMoveIfItIsInsideBoard(currentMove);
		}
	};

	auto getPawnMoves = [&]() {
		int pieceFactor = piece.getColor() == WHITE ? -1 : 1;

		if (lastBoardCoords[0] + pieceFactor >= 0 && lastBoardCoords[0] + pieceFactor < squares.size()) {
			if ((*this)[lastBoardCoords[0] + pieceFactor][lastBoardCoords[1]] == EMPTY) {
				validMoves.push_back({ lastBoardCoords[0] + pieceFactor, lastBoardCoords[1] });
			}

			if (getSquareColor((*this)[lastBoardCoords[0] + pieceFactor][lastBoardCoords[1] - 1]) != -1 != WHITE) {
				validMoves.push_back({ lastBoardCoords[0] + pieceFactor, lastBoardCoords[1] - 1 });
			}

			if (getSquareColor((*this)[lastBoardCoords[0] + pieceFactor][lastBoardCoords[1] + 1]) != -1 != WHITE) {
				validMoves.push_back({ lastBoardCoords[0] + pieceFactor, lastBoardCoords[1] + 1 });
			}
		}

		if (lastBoardCoords[0] + 2 * pieceFactor >= 0 && lastBoardCoords[0] + 2 * pieceFactor < squares.size()) {
			bool isInFirstRank = false;
			if (
				piece.getColor() == WHITE && lastBoardCoords[0] == 6 || 
				piece.getColor() == BLACK && lastBoardCoords[0] == 1
			) {
				isInFirstRank = true;
			}

			if ((*this)[lastBoardCoords[0] + 2 * pieceFactor][lastBoardCoords[1]] == EMPTY && isInFirstRank) {
				pushMoveIfItIsInsideBoard({ lastBoardCoords[0] + 2 * pieceFactor, lastBoardCoords[1] });
			}
		}
	};

	switch (piece.getType()) {
		case KING:
			getKingMoves();		
			break;

		case KNIGHT:
			getKnightMoves();
			break;

		case BISHOP: 
			getBishopMoves();
			break;
		
		case ROOK:
			std::cout << "Get our moves" << std::endl; 
			getRookMoves();	
			break;

		case QUEEN:
			getBishopMoves();
			getRookMoves();
			break;
	
		case PAWN:
			getPawnMoves();
			break;
	}

	return validMoves;
}

void Board::indicateValidMoves(Piece piece) {
	std::vector<std::vector<int>> moves = getValidMoves(piece);

	for (auto move : moves) {
		if (getSquareColor(move) == piece.getColor()) continue;
		
		MoveIndicator indicator(toScreenCoords(move));
		if (getSquareColor(move) != piece.getColor() && (*this)[move] != EMPTY) {
			indicator.indicateCapture();
		}

		indicators.push_back(indicator);
	}
}

bool Board::isMoveValid(Piece piece, std::vector<int> move) {
	std::vector<std::vector<int>> validMoves = getValidMoves(piece);

	if (isKingsideCastle(piece, move) || isQueensideCastle(piece, move)) {
		return true;
	}

	for (std::vector<int> validMove : validMoves) {
		// TODO: Put this condition on checkValidMoves function instead
		if (validMove[0] >= 0 && validMove[1] <= squares.size() - 1 && validMove == move) {
			return true;
		}
	}

	return false;
}

bool Board::isKingsideCastle(Piece piece, std::vector<int> move) {
	if (piece.getType() == KING) {
		for (auto move : playedMoves) {
			// If the king or the rook has moved, it's invalid
			if (move[piece.getType()][0] == 'K' || move[piece.getType()][0] == 'R') {
				return false;
			}
		}

		// If there's any pieces between the king and the rook, it's invalid
		if ((*this)["f1"] != EMPTY || (*this)["g1"] != EMPTY) {
			return false;
		}

		if (piece.getColor() == WHITE && (*this)[move] == ROOK || piece.getColor() == BLACK && (*this)[move] == -ROOK) {
			// Rook is at the right of the board
			if (move[1] == 7) {
				return true;
			}
		}
	}

	return false;
}

bool Board::isQueensideCastle(Piece piece, std::vector<int> move) {
	if (!isKingsideCastle(piece, move)) {
		// If there's any pieces between the king and the rook, it's invalid
		if ((*this)["d1"] != EMPTY || (*this)["c1"] != EMPTY || (*this)["b1"] != EMPTY) {
			return false;
		}

		// Rook is at the left of the board
		if (move[1] == 0) {
			return true;
		}
	}

	return false;
}

bool Board::isKingChecked(int color) {
	std::vector<int> kingSquare;

	for (int rank = 0; rank < squares.size(); rank++) {
		for (int file = 0; file < squares[rank].size(); file++) {
			int colorFactor = color == WHITE ? 1 : -1;
			if ((*this)[rank][file] == (KING * colorFactor)) {
				kingSquare = { rank, file };
			}
		}
	}

	for (Piece piece : pieces) {
		if (piece.getType() == KING || piece.getColor() == color) continue;

		// TODO: Fix seg fault here
		std::vector<std::vector<int>> validMoves = getValidMoves(piece);
	
		if (validMoves.size() == 0) continue;

		for (std::vector<int> validMove : validMoves) {
			if (validMove == kingSquare) {
				return true;
			}
		}
	}

	return false;
}

void Board::indicateCheck() {
	auto findKing = [&](int color) {
		for (Piece piece : pieces) {
			if (piece.getType() == KING && piece.getColor() == color) {
				return piece.getBoardCoords();
			}
		}

		return (std::vector<int>(0, 0));
	};

	/*if (isKingChecked(WHITE)) {
		std::vector<int> kingCoords = findKing(WHITE);
		MoveIndicator checkIndicator(toScreenCoords(kingCoords));
		checkIndicator.indicateCheck();
		indicators.push_back(checkIndicator);
	} else if (isKingChecked(BLACK)) {
		std::vector<int> kingCoords = findKing(BLACK);
		MoveIndicator checkIndicator(toScreenCoords(kingCoords));
		checkIndicator.indicateCheck();
		indicators.push_back(checkIndicator);
	}*/	
}

bool Board::makeMove(Piece& piece, std::vector<int> move) {
	indicators.clear();

	// Can't move if your king is checked
	/*if (isKingChecked(piece.getColor())) {
		
		return false;
	}*/

	std::vector<int> lastCoords = piece.getLastBoardCoords();
	int colorFactor = piece.getColor() == WHITE ? 1 : -1;

	if (!isMoveValid(piece, move)) {
		(*this)[lastCoords] = piece.getType() * colorFactor;
		loadPieces();
		return false;
	}

	bool hasCastled = false;
	int castleSide = 0;
	if (isKingsideCastle(piece, move)) {
		castleSide = KINGSIDE;
		hasCastled = true;
	} else if (isQueensideCastle(piece, move)) {
		castleSide = QUEENSIDE;
		hasCastled = true;
	}

	bool isCapture = false;
	if ((*this)[move] != EMPTY && !hasCastled) {
		int squareColor = getSquareColor((*this)[move]);
		if (squareColor == piece.getColor()) { 
			(*this)[lastCoords] = piece.getType() * colorFactor;
			loadPieces();
			return false; 
		}

		isCapture = true;
	}

	std::string chessNote = "";

	if (isCapture) {
		chessNote = piece.getCoordsInChessNotationWithCapture();
	} else if (hasCastled) {
		if (castleSide == KINGSIDE) {
			chessNote = "O-O";
		} else {
			chessNote = "O-O-O";
		}
	} else {
		chessNote = piece.getCoordsInChessNotation();
	}

	if (playedMoves.size() == 0) {
		playedMoves.resize(1);
		playedMoves[0].resize(2);
	}

	if (playedMoves[playedMoves.size() - 1][WHITE] == "") {
		playedMoves[playedMoves.size() - 1][WHITE] = chessNote;
	} else {
		playedMoves[playedMoves.size() - 1][BLACK] = chessNote;
		playedMoves.resize(playedMoves.size() + 1);
		playedMoves[playedMoves.size() - 1].resize(2);
	}

	printMoveHistory();

	if (hasCastled) {
		(*this)[lastCoords[0]][lastCoords[1] + 2 * castleSide] = KING * colorFactor;
		(*this)[lastCoords[0]][lastCoords[1] + 1 * castleSide] = ROOK * colorFactor;
		(*this)["e1"] = EMPTY;
		(*this)[castleSide == KINGSIDE ? "h1" : "a1"] = EMPTY;
		loadPieces();
		return true;
	}
	
	(*this)[lastCoords] = EMPTY;
	(*this)[move] = piece.getType() * colorFactor;
	loadPieces();

	return true;
}

void Board::draw(sf::RenderWindow& window) {
	window.draw(boardSprite);

	for (MoveIndicator indicator : indicators) {
		indicator.draw(window);
	}

	for (Piece piece : pieces) {
		piece.draw(window);
	}
}
