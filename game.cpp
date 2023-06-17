#include "game.h"
#include <iostream>
bool inGrid(int x) {
    return (x >= 0) && (x < 8);
}
bool operator==(const Coord& a, const Coord& b)
{
    return (a.x == b.x) && (a.y == b.y);
}
bool operator==(const Move& a, const Move& b)
{
    return (a.origin == b.origin) && (a.target == b.target);
}
void printMove(Move move) {
    // TODO: tidy this up because this is long and ugly
    switch(move.origin.x) {
        case 0:
        std::cout << "a";
        break;
        case 1:
        std::cout << "b";
        break;
        case 2:
        std::cout << "c";
        break;
        case 3:
        std::cout << "d";
        break;
        case 4:
        std::cout << "e";
        break;
        case 5:
        std::cout << "f";
        break;
        case 6:
        std::cout << "g";
        break;
        case 7:
        std::cout << "h";
        break;
    }
    std::cout << 8 - move.origin.y;
    switch(move.target.x) {
        case 0:
        std::cout << "a";
        break;
        case 1:
        std::cout << "b";
        break;
        case 2:
        std::cout << "c";
        break;
        case 3:
        std::cout << "d";
        break;
        case 4:
        std::cout << "e";
        break;
        case 5:
        std::cout << "f";
        break;
        case 6:
        std::cout << "g";
        break;
        case 7:
        std::cout << "h";
        break;
    }
    std::cout << 8 - move.target.y << std::endl;
}
std::vector<Move> Game::generatePseudoLegalMoves(bool color, bool excludeCastling=false) {
    std::vector<Move> pseudoLegalMoves;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            int piece = this->board[i][j];
            bool isPiece = this->board[i][j] != 0;
            if(isPiece && this->getPieceColor(piece) == color) {
                std::vector<Move> piecePseudoLegalMoves = this->generatePseudoLegalMovesForPieceAtCoord(Coord{j, i}, excludeCastling);
                pseudoLegalMoves.insert(pseudoLegalMoves.begin(), piecePseudoLegalMoves.begin(), piecePseudoLegalMoves.end());
            }
        }
    }
    return pseudoLegalMoves;
}
bool Game::canPieceOccupy(int piece, Coord coord) {
    return (this->board[coord.y][coord.x] == 0) || (this->getPieceColor(this->board[coord.y][coord.x]) != this->getPieceColor(piece));
}
bool Game::isCoordEmpty(Coord coord) {
    return (this->board[coord.y][coord.x] == 0);
}
Coord Game::getKingCoord(bool color) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            int piece = this->board[i][j];
            if((piece == K) && color) return Coord {j, i};
            if((piece == k) && !color) return Coord {j, i};
        }
    }
    return Coord {-1, -1}; // no king found, throw an error eventually?
}
int Game::getPawnStartingRank(bool color) {
    if(color) return 6;
    else return 1;
}
std::vector<Move> Game::generatePseudoLegalMovesForPieceAtCoord(Coord coord, bool excludeCastling=false) {
    std::vector<Move> pseudoLegalMoves;
    int piece = this->board[coord.y][coord.x];
    switch(piece) {
        case r:
        case R: {
            for(int i = coord.x+1; i < 8; i++) {
                Coord target = Coord {i, coord.y};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = coord.x-1; i >= 0; i--) {
                Coord target = Coord {i, coord.y};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = coord.y+1; i < 8; i++) {
                Coord target {coord.x, i};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = coord.y-1; i >= 0; i--) {
                Coord target {coord.x, i};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            break;
        }
        case n:
        case N: {
            if(inGrid(coord.x + 2) && inGrid(coord.y + 1) && this->canPieceOccupy(piece, Coord {coord.x + 2, coord.y + 1}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x + 2, coord.y + 1}});
            if(inGrid(coord.x + 2) && inGrid(coord.y - 1) && this->canPieceOccupy(piece, Coord {coord.x + 2, coord.y - 1}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x + 2, coord.y - 1}});
            if(inGrid(coord.x + 1) && inGrid(coord.y + 2) && this->canPieceOccupy(piece, Coord {coord.x + 1, coord.y + 2}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x + 1, coord.y + 2}});
            if(inGrid(coord.x + 1) && inGrid(coord.y - 2) && this->canPieceOccupy(piece, Coord {coord.x + 1, coord.y - 2}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x + 1, coord.y - 2}});
            if(inGrid(coord.x - 2) && inGrid(coord.y + 1) && this->canPieceOccupy(piece, Coord {coord.x - 2, coord.y + 1}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x - 2, coord.y + 1}});
            if(inGrid(coord.x - 2) && inGrid(coord.y - 1) && this->canPieceOccupy(piece, Coord {coord.x - 2, coord.y - 1}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x - 2, coord.y - 1}});
            if(inGrid(coord.x - 1) && inGrid(coord.y + 2) && this->canPieceOccupy(piece, Coord {coord.x - 1, coord.y + 2}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x - 1, coord.y + 2}});
            if(inGrid(coord.x - 1) && inGrid(coord.y - 2) && this->canPieceOccupy(piece, Coord {coord.x - 1, coord.y - 2}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x - 1, coord.y - 2}});
            break;
        }
        case b:
        case B: {
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x + i, coord.y + i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x - i, coord.y + i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x + i, coord.y - i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x - i, coord.y - i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            break;
        }
        case k:
        case K: {
            bool color = this->getPieceColor(piece);
            if(inGrid(coord.x + 1) && inGrid(coord.y) && this->canPieceOccupy(piece, Coord {coord.x + 1, coord.y}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x + 1, coord.y}});
            if(inGrid(coord.x - 1) && inGrid(coord.y)  && this->canPieceOccupy(piece, Coord {coord.x -1, coord.y}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x - 1, coord.y}});
            if(inGrid(coord.x) && inGrid(coord.y + 1)  && this->canPieceOccupy(piece, Coord {coord.x, coord.y + 1}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x, coord.y + 1}});
            if(inGrid(coord.x) && inGrid(coord.y - 1)  && this->canPieceOccupy(piece, Coord {coord.x, coord.y - 1}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x, coord.y - 1}});
            if(inGrid(coord.x + 1) && inGrid(coord.y + 1)  && this->canPieceOccupy(piece, Coord {coord.x + 1, coord.y + 1}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x + 1, coord.y + 1}});
            if(inGrid(coord.x + 1) && inGrid(coord.y - 1)  && this->canPieceOccupy(piece, Coord {coord.x + 1, coord.y - 1}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x + 1, coord.y - 1}});
            if(inGrid(coord.x - 1) && inGrid(coord.y + 1)  && this->canPieceOccupy(piece, Coord {coord.x - 1, coord.y + 1}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x - 1, coord.y + 1}});
            if(inGrid(coord.x - 1) && inGrid(coord.y - 1)  && this->canPieceOccupy(piece, Coord {coord.x - 1, coord.y - 1}))
                pseudoLegalMoves.push_back(Move {coord, Coord {coord.x - 1, coord.y - 1}});
            // castling kingside
            if(!excludeCastling) {
                if(
                    ((color && this->canWhiteCastleKingside) || (!color && this->canBlackCastleKingside))
                    && this->isCoordEmpty(Coord{coord.x+1,coord.y})
                    && this->isCoordEmpty(Coord{coord.x+2,coord.y})
                    && !this->isAttacked(color, Coord{coord.x+1,coord.y}) 
                    && !this->isAttacked(color, Coord{coord.x+2,coord.y})
                ) {
                    Move castle;
                    castle.origin = coord;
                    castle.target = Coord {coord.x + 2, coord.y};
                    castle.castling = true;
                    pseudoLegalMoves.push_back(castle);
                }
                // castling queenside
                if(
                    ((color && this->canWhiteCastleQueenside) || (!color && this->canBlackCastleQueenside))
                    && this->isCoordEmpty(Coord{coord.x-1,coord.y})
                    && this->isCoordEmpty(Coord{coord.x-2,coord.y})
                    && !this->isAttacked(color, Coord{coord.x-1,coord.y}) 
                    && !this->isAttacked(color, Coord{coord.x-2,coord.y})
                ) {
                    Move castle;
                    castle.origin = coord;
                    castle.target = Coord {coord.x - 2, coord.y};
                    castle.castling = true;
                    pseudoLegalMoves.push_back(castle);
                }
            }
            
            break;
        }
        case q:
        case Q: {
            for(int i = coord.x+1; i < 8; i++) {
                Coord target = Coord {i, coord.y};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = coord.x-1; i >= 0; i--) {
                Coord target = Coord {i, coord.y};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = coord.y+1; i < 8; i++) {
                Coord target {coord.x, i};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = coord.y-1; i >= 0; i--) {
                Coord target {coord.x, i};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x + i, coord.y + i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x - i, coord.y + i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x + i, coord.y - i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x - i, coord.y - i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
                if(!this->isCoordEmpty(target)) break;
            }
            break;
        }
        case p:
        case P: {
            // TODO: en-passant rule
            int multiplier = 1;
            bool color = this->getPieceColor(piece);
            if(color) multiplier = -1;
            Coord twoSquares {coord.x, coord.y + 2 * multiplier};
            Coord oneSquare {coord.x, coord.y + 1 * multiplier};
            Coord leftDiagonal {coord.x - 1, coord.y+ 1 * multiplier};
            Coord rightDiagonal {coord.x + 1, coord.y+ 1 * multiplier};
            if(this->getPawnStartingRank(color) == coord.y && this->isCoordEmpty(twoSquares) && this->isCoordEmpty(oneSquare)) {
                pseudoLegalMoves.push_back(Move {coord, twoSquares});
            }
            if(this->canPieceOccupy(piece, leftDiagonal) && !this->isCoordEmpty(leftDiagonal)) {
                pseudoLegalMoves.push_back(Move {coord, leftDiagonal});
            }
            if(this->canPieceOccupy(piece, rightDiagonal) && !this->isCoordEmpty(rightDiagonal)) {
                pseudoLegalMoves.push_back(Move {coord, rightDiagonal});
            }
            if(this->canPieceOccupy(piece, oneSquare) && this->isCoordEmpty(oneSquare)) {
                pseudoLegalMoves.push_back(Move {coord, oneSquare});
            }
            break;
        }
        default: {
            break;
        }
    }
    return pseudoLegalMoves;
}
bool Game::isAttacked(bool color, Coord coord) {
    // Returns if a given square is attacked for the given color.
    std::vector<Move> opponentPseudoLegalMoves = this->generatePseudoLegalMoves(!color, true);
    for(auto opponentMove: opponentPseudoLegalMoves) {
        if(opponentMove.target == coord) {
            return true;
        }
    }
    return false;
}
bool Game::isCheck(bool color) {
    return this->isAttacked(color, this->getKingCoord(color));
}
std::vector<Move> Game::getLegalMoves(bool color) {
    // Simulate the move on the board.
    // Then, check if the king's square is part of the opponent's legal moves.
    // If it is, then filter out the move from the list.
    // Restore the board to its initial position by undoing the move. 
    // Repeat this for all the pseudo legal moves that need to be verified.
    std::vector<Move> pseudoLegalMoves = this->generatePseudoLegalMoves(color);
    std::vector<Move> legalMoves;
    for(auto move: pseudoLegalMoves) {
        bool isLegal = true;
        // make the move on the board, store the piece on target square to place it back at the end
        int targetPiece = this->board[move.target.y][move.target.x];
        this->board[move.target.y][move.target.x] = board[move.origin.y][move.origin.x];
        this->board[move.origin.y][move.origin.x] = 0;
        // if king is in check, move is illegal
        if(this->isCheck(color)) {
            isLegal = false;
        }
        // undo move on board
        this->board[move.origin.y][move.origin.x] = this->board[move.target.y][move.target.x];
        this->board[move.target.y][move.target.x] = targetPiece;
        if(isLegal)
            legalMoves.push_back(move);
    }
    return legalMoves;
}
bool Game::getPieceColor(int piece) {
    return piece < 7; // is the arbitrarily assigned number below 7?
}
void Game::handleMove(Move move) {
    // This function is meant to handle moves that affect the game state in a special way, and apply the according side effects.
    std::cout << move.castling << std::endl;
    if(move.castling) {
        bool kingside = move.target.x > 4;
        this->board[7][move.target.x-1] = this->board[7][7];
        this->board[7][7] = 0;
        std::cout << "castled!" << std::endl;
    }
    if(this->board[move.target.y][move.target.x] == K) {
        this->canWhiteCastleKingside = false;
        this->canWhiteCastleQueenside = false;
    }
    if(this->board[move.target.y][move.target.x] == k) {
        this->canBlackCastleKingside = false;
        this->canBlackCastleQueenside = false;
    }
    if(move.origin.x == 7 && move.origin.y == 7) {
        this->canWhiteCastleKingside = false;
    }
    if(move.origin.x == 0 && move.origin.y == 7) {
        this->canWhiteCastleQueenside = false;
    }
    if(move.origin.x == 7 && move.origin.y == 0) {
        this->canBlackCastleKingside = false;
    }
    if(move.origin.x == 0 && move.origin.y == 0) {
        this->canBlackCastleQueenside = false;
    }
}
MoveResult Game::makeMove(Move& move) {
    // TODO: set kingside and queenside accordingly
    bool pieceColor = this->getPieceColor(this->board[move.origin.y][move.origin.x]); 
    bool isPieceTurn = pieceColor == this->turn;
    if(isPieceTurn) {
        bool isMoveLegal =  false;
        std::vector<Move> legalMoves = this->getLegalMoves(this->turn);
        for(int i = 0; i < legalMoves.size(); i++) {
            Move legalMove = legalMoves.at(i);
            if (legalMove == move) {
                isMoveLegal = true;
                move = legalMove; // include additional information other than origin & target.
                break;
            }
        }
        if(!isMoveLegal) {
            return MoveResult {false, false, false};
        }
        this->board[move.target.y][move.target.x] = board[move.origin.y][move.origin.x];
        this->board[move.origin.y][move.origin.x] = 0;
        this->handleMove(move);
        this->turn = !this->turn; // flip turn
        return MoveResult {true, move.castling && move.target.x > 4, move.castling && move.target.x < 4};
    }
    return MoveResult {false, false, false};
}
