#if !defined CSZD_CHESSMOVE_HEADER
#define CSZD_CHESSMOVE_HEADER

#include <bitset>
#include "cmdsuzdal/bbdefines.h"
#include "cmdsuzdal/chessdefines.h"

namespace cSzd
{

    // Chess Move. 32-bits wide bitset with the following format:
    //
    //  bits[0..2]   = the moved piece (0..5, cannot be invalid)
    //  bits[4..6]   = in case of opposite army piece taken, the taken piece
    //                 (InvalidPiece if no piece taken)
    //  bits[8..10]  = in case of promotion, the piece chosen after promotion
    //                 (InvalidPiece if no promotion)
    //  bits[12..17] = the start Cell (0...63, cannot be invalid)
    //  bits[18..23] = the destination Cell (0...63, cannot be invalid)
    //  bits[24..30] = the en passant Cell (InvalidCell if no en-passant)
    //  bits[31]     = invalid move flag (1 = invalid)
    //
    //  Examples:
    //    - Pawn e2 to e3
    //      0 1000000 010100 001100 0110 0110 0101 = 0x4050C665
    using ChessMove = std::bitset<32>;
    constexpr ChessMove InvalidMove = 0x80000000;

    constexpr unsigned int MovedPieceOffset = 0;
    constexpr unsigned int TakenPieceOffset = 4;
    constexpr unsigned int PromotedPieceOffset = 8;
    constexpr unsigned int StartCellOffset = 12;
    constexpr unsigned int DestinationCellOffset = 18;
    constexpr unsigned int EnPassantCellOffset = 24;

    constexpr unsigned int PieceMask = 0x0007;
    constexpr unsigned int ValidCellMask = 0x003F;
    constexpr unsigned int ValidAndInvalidCellMask = 0x007F;

    ChessMove chessMove(Piece movedPiece, Cell startCell, Cell destCell,
                        Piece takenPiece = InvalidPiece,
                        Piece promotedPiece = InvalidPiece);
    Cell computeEnPassant(Cell from, Cell to);
}

#endif // #if !defined CSZD_CHESSMOVE_HEADER