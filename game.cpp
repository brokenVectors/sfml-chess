#include "game.h"
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
std::vector<Move> Game::generatePseudoLegalMoves(bool color) {
    std::vector<Move> pseudoLegalMoves;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            int piece = this->board[i][j];
            bool isPiece = this->board[i][j] != 0;
            if(isPiece && this->getPieceColor(piece) == color) {
                std::vector<Move> piecePseudoLegalMoves = this->generatePseudoLegalMovesForPieceAtCoord(Coord{j, i});
                pseudoLegalMoves.insert(pseudoLegalMoves.begin(), piecePseudoLegalMoves.begin(), piecePseudoLegalMoves.end());
            }
        }
    }
    return pseudoLegalMoves;
}
bool Game::canPieceOccupy(int piece, Coord coord) {
    return (this->board[coord.y][coord.x] == 0) || (this->getPieceColor(this->board[coord.y][coord.x]) != this->getPieceColor(piece));
}
bool Game::isEnemyOnSquare(int piece, Coord coord) {
    return (this->getPieceColor(this->board[coord.y][coord.x]) != this->getPieceColor(piece));
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
std::vector<Move> Game::generatePseudoLegalMovesForPieceAtCoord(Coord coord) {
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
            }
            for(int i = coord.x-1; i >= 0; i--) {
                Coord target = Coord {i, coord.y};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            for(int i = coord.y+1; i < 8; i++) {
                Coord target {coord.x, i};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            for(int i = coord.y-1; i >= 0; i--) {
                Coord target {coord.x, i};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
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
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x - i, coord.y + i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x + i, coord.y - i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x - i, coord.y - i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            break;
        }
        case k:
        case K: {
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
            break;
        }
        case q:
        case Q: {
            for(int i = coord.x+1; i < 8; i++) {
                Coord target = Coord {i, coord.y};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            for(int i = coord.x-1; i >= 0; i--) {
                Coord target = Coord {i, coord.y};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            for(int i = coord.y+1; i < 8; i++) {
                Coord target {coord.x, i};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            for(int i = coord.y-1; i >= 0; i--) {
                Coord target {coord.x, i};
                if(this->canPieceOccupy(piece, target))
                    pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x + i, coord.y + i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x - i, coord.y + i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x + i, coord.y - i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
            }
            for(int i = 1; i < 7; i++) {
                Coord target {coord.x - i, coord.y - i};
                if(inGrid(target.x) && inGrid(target.y) && this->canPieceOccupy(piece, target)) pseudoLegalMoves.push_back(Move {coord, target});
                else break;
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
            if(this->getPawnStartingRank(color) == coord.y) {
                pseudoLegalMoves.push_back(Move {coord, twoSquares});
            }
            if(this->isEnemyOnSquare(piece, leftDiagonal)) {
                pseudoLegalMoves.push_back(Move {coord, leftDiagonal});
            }
            if(this->isEnemyOnSquare(piece, rightDiagonal)) {
                pseudoLegalMoves.push_back(Move {coord, rightDiagonal});
            }
            pseudoLegalMoves.push_back(Move {coord, oneSquare});
            break;
        }
        default: {
            break;
        }
    }
    return pseudoLegalMoves;
}

std::vector<Move> Game::getLegalMoves(bool color) {
    // This function should just filter out the moves that put one's own king in check.
    // To do this, the move should first be played out, simulated on the board.
    // Then, check if the king's square is part of the opponent's legal moves.
    // If it is, then filter out the move from the list.
    // Restore the board to its initial position by undoing the move. 
    // Repeat this for all the pseudo legal moves that need to be verified.
    std::vector<Move> pseudoLegalMoves = this->generatePseudoLegalMoves(color);
    //std::remove_if(pseudoLegalMoves.begin(), pseudoLegalMoves.end(), [this]() {
    //});
    return pseudoLegalMoves;
}
bool Game::getPieceColor(int piece) {
    return piece < 7; // is the arbitrarily assigned number below 7?
}
bool Game::makeMove(Move move) {
    bool pieceColor = this->getPieceColor(this->board[move.origin.y][move.origin.x]); 
    bool isPieceTurn = pieceColor == this->turn;
    if(isPieceTurn) {
        bool isMoveLegal =  false;
        std::vector<Move> legalMoves = this->getLegalMoves(this->turn);
        for(int i = 0; i < legalMoves.size(); i++) {
            Move legalMove = legalMoves.at(i);
            if (legalMove == move) {
                isMoveLegal = true;
                break;
            }
        }
        if(!isMoveLegal) {
            return false;
        }
        this->board[move.target.y][move.target.x] = board[move.origin.y][move.origin.x];
        this->board[move.origin.y][move.origin.x] = 0;
        this->turn = !this->turn; // flip turn
        return true;
    }
    return false;
}
