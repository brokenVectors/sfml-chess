#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>
constexpr int P = 6;
constexpr int R = 5;
constexpr int N = 4;
constexpr int B = 3;
constexpr int Q = 2;
constexpr int K = 1;
constexpr int p = 12;
constexpr int r = 11;
constexpr int n = 10;
constexpr int b = 9;
constexpr int q = 8;
constexpr int k = 7;
struct Coord {
    int x;
    int y;
};
struct Move {
    Coord origin;
    Coord target;
    Coord enPassantSquare = Coord {-1, -1}; // -1, -1 = no en passant
    int promotionPiece = -1; // -1 = no promotion
    bool castling = false;
};
struct MoveResult {
    bool success = false;
    bool kingside = false;
    bool queenside = false;
    Coord enPassantSquare;
    int promotionPiece = -1;
};
class Game {
    protected:
    
    public:
    int board[8][8] {
        r, n, b, q, k, b, n, r, 
        p, p, p, p, p, p, p, p, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        P, P, P, P, P, P, P, P, 
        R, N, B, Q, K, B, N, R, 
    };
    bool turn = true; // is it white to play?
    bool canWhiteCastleKingside = true;
    bool canWhiteCastleQueenside = true;
    bool canBlackCastleKingside = true;
    bool canBlackCastleQueenside = true;
    Move lastMove;
    std::vector<Move> getLegalMoves(bool color); // returns legal moves for a given side
    std::vector<Move> generatePseudoLegalMoves(bool color, bool excludeCastling); // generates pseudo-legal moves for a given side
    std::vector<Move> generatePseudoLegalMovesForPieceAtCoord(Coord coord, bool excludeCastling); // generates pseudo-legal moves for a given piece at a given coordinate
    MoveResult makeMove(Move& move); // makes a move on the board after checking if it's legal, returns the legality
    bool getPieceColor(int piece);
    bool canPieceOccupy(int piece, Coord coord);
    Coord getKingCoord(bool color);
    bool isCoordEmpty(Coord coord);
    bool isCheck(bool color); // Returns if a given color is in check.
    int getPawnStartingRank(bool color);
    int getBackrank(bool color);
    bool isAttacked(bool color, Coord coord);
    void handleMove(Move move);
};
void printMove(Move move);
#endif