#include "board.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdlib>

Board::Board() {}

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

	// TODO: Make a loop through all the squares and skip empty squares
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

std::vector<int> Board::operator[](int at) const {
	return squares[at];
}

int Board::operator[](std::vector<int> at) const {
	return squares[at[0]][at[1]];
}

std::vector<int>& Board::operator[](int at) {
	return squares[at];
}

int& Board::operator[](std::vector<int> at) {
	return squares[at[0]][at[1]];
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
		if (playedMove.size() == 2) {
			std::cout << i << ". " << playedMove[0] << " " << playedMove[1] << std::endl;
		} else if (playedMove.size() == 1) {
			std::cout << i << ". " << playedMove[0] << std::endl;
		}
		i++;
	}
}

bool Board::isMoveValid(Piece piece, std::vector<int> move) {
	std::vector<std::vector<int>> validMoves;
	std::vector<int> lastBoardCoords = piece.getLastBoardCoords();

	auto getKingMoves = [&]() {
		validMoves.push_back(squareAbove(lastBoardCoords));
		validMoves.push_back(squareAtLeft(squareAbove(lastBoardCoords)));
		validMoves.push_back(squareAtRight(squareAbove(lastBoardCoords)));
		validMoves.push_back(squareAtLeft(lastBoardCoords));
		validMoves.push_back(squareAtRight(lastBoardCoords));
		validMoves.push_back(squareBelow(lastBoardCoords));
		validMoves.push_back(squareAtLeft(squareBelow(lastBoardCoords)));
		validMoves.push_back(squareAtRight(squareBelow(lastBoardCoords)));
	};

	auto getKnightMoves = [&]() {
		validMoves.push_back(squareAtLeft(squareAbove(squareAbove(lastBoardCoords))));
		validMoves.push_back(squareAtRight(squareAbove(squareAbove(lastBoardCoords))));
		validMoves.push_back(squareAtLeft(squareBelow(squareBelow(lastBoardCoords))));
		validMoves.push_back(squareAtRight(squareBelow(squareBelow(lastBoardCoords))));
		validMoves.push_back(squareAbove(squareAtLeft(squareAtLeft(lastBoardCoords))));
		validMoves.push_back(squareBelow(squareAtLeft(squareAtLeft(lastBoardCoords))));
		validMoves.push_back(squareAbove(squareAtRight(squareAtRight(lastBoardCoords))));
		validMoves.push_back(squareBelow(squareAtRight(squareAtRight(lastBoardCoords))));
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

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);	
			currentMove = squareAtRight(currentMove);
		}

		if (getSquareColor(currentMove) != -1 != piece.getColor()) {
			if (isInsideBoard(currentMove)) {
				validMoves.push_back(currentMove);
			}
		}

		currentMove = squareAtLeft(lastBoardCoords);

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);	
			currentMove = squareAtLeft(currentMove);
		}

		if (getSquareColor(currentMove) != piece.getColor()) {
			if (isInsideBoard(currentMove)) {
				validMoves.push_back(currentMove);
			}
		}

		currentMove = squareAbove(lastBoardCoords);

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);
			currentMove = squareAbove(currentMove);
		}

		if (getSquareColor(currentMove) != piece.getColor()) {
			if (isInsideBoard(currentMove)) {
				validMoves.push_back(currentMove);
			}
		}

		currentMove = squareBelow(lastBoardCoords);

		while (isInsideBoard(currentMove) && (*this)[currentMove] == EMPTY) {
			validMoves.push_back(currentMove);	
			currentMove = squareBelow(currentMove);
		}

		if (getSquareColor(currentMove) != piece.getColor()) {
			if (isInsideBoard(currentMove)) {
				validMoves.push_back(currentMove);
			}
		}
	};

	auto getPawnMoves = [&]() {
		int pieceFactor = piece.getColor() == WHITE ? -1 : 1;

		if (lastBoardCoords[0] + pieceFactor >= 0 && lastBoardCoords[0] + pieceFactor < squares.size()) {
			if ((*this)[{ lastBoardCoords[0] + pieceFactor, lastBoardCoords[1] }] == EMPTY) {
				validMoves.push_back({ lastBoardCoords[0] + pieceFactor, lastBoardCoords[1] });
			}

			if (getSquareColor((*this)[{ lastBoardCoords[0] + pieceFactor, lastBoardCoords[1] - 1 }]) != -1 != WHITE) {
				validMoves.push_back({ lastBoardCoords[0] + pieceFactor, lastBoardCoords[1] - 1 });
			}

			if (getSquareColor((*this)[{ lastBoardCoords[0] + pieceFactor, lastBoardCoords[1] + 1 }]) != -1 != WHITE) {
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

			if ((*this)[{ lastBoardCoords[0] + 2 * pieceFactor, lastBoardCoords[1] }] == EMPTY && isInFirstRank) {
				validMoves.push_back({ lastBoardCoords[0] + 2 * pieceFactor, lastBoardCoords[1] });
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

	for (std::vector<int> validMove : validMoves) {
		if (validMove[0] >= 0 && validMove[1] <= squares.size() - 1 && validMove == move) {
			return true;
		}
	}

	return false;
}

bool Board::makeMove(Piece& piece, std::vector<int> move) {
	std::vector<int> lastCoords = piece.getLastBoardCoords();
	int colorFactor = piece.getColor() == WHITE ? 1 : -1;

	if (!isMoveValid(piece, move)) {
		(*this)[lastCoords] = piece.getType() * colorFactor;
		loadPieces();
		return false;
	}

	bool isCapture = false;
	if ((*this)[move] != EMPTY) {
		int squareColor = getSquareColor((*this)[move]);
		if (squareColor == piece.getColor()) { 
			(*this)[lastCoords] = piece.getType() * colorFactor;
			loadPieces();
			return false; 
		}

		isCapture = true;
	}

	std::string chessNote = isCapture ? piece.getCoordsInChessNotationWithCapture() : piece.getCoordsInChessNotation();

	if (playedMoves.size() == 0) {
		playedMoves.resize(playedMoves.size() + 1);
	}

	if (playedMoves[playedMoves.size() - 1].size() == 0) {
		playedMoves[playedMoves.size() - 1].resize(1);
		playedMoves[playedMoves.size() - 1][0] = chessNote;
	} else {
		playedMoves[playedMoves.size() - 1].resize(2);
		playedMoves[playedMoves.size() - 1][1] = chessNote;
		playedMoves.resize(playedMoves.size() + 1);
	}

	printMoveHistory();

	(*this)[lastCoords] = EMPTY;
	(*this)[move] = piece.getType() * colorFactor;
	loadPieces();

	return true;
}

void Board::draw(sf::RenderWindow& window) {
	window.draw(boardSprite);

	for (int i = 0; i < pieces.size(); i++) {
		pieces[i].draw(window);
	}
}