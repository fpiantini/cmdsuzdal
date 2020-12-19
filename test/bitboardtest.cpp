#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "cmdsuzdal/bitboard.h"

using namespace std;
using namespace testing;

namespace cSzd
{
    // --------------------------------------------------------
    TEST(BBTester, AfterDefinitionBitboardIsEmpty)
    {
        BitBoard bb;
        ASSERT_EQ(bb, BitBoard(EmptyBB));
    }
    // --------------------------------------------------------
    TEST(BBTester, AfterInitializationWithAnInvalidCellBitBoardIsEmpty)
    {
        BitBoard bb{InvalidCell};
        ASSERT_EQ(bb, BitBoard(EmptyBB));
    }

    // --------------------------------------------------------
    TEST(BBTester, AfterInitializationWithAnInvalidCellsArrayBitBoardIsEmpty)
    {
        BitBoard bb({InvalidCell, InvalidCell, InvalidCell});
        ASSERT_EQ(bb, BitBoard(EmptyBB));
    }

    // --------------------------------------------------------
    TEST(BBTester, AfterInitializationWithGoodAndInvalidCellsArrayBitBoardIsOK)
    {
        BitBoard bb({InvalidCell, e1, InvalidCell, f5, InvalidCell, h8});
        ASSERT_EQ(bb, BitBoard({e1, f5, h8}));
    }

    // --------------------------------------------------------
    TEST(BBTester, SetOfInvalidCellsHasNoImpactsOnBitBoard)
    {
        BitBoard bb;
        bb.setCell({InvalidCell, c7, InvalidCell, d5});
        bb.setCell(InvalidCell);
        bb.setCell(g2);
        ASSERT_EQ(bb, BitBoard({c7, d5, g2}));
    }

    TEST(BBTester, Cell_0_0_CorrespondToA1)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell(f_a, r_1), BitBoard(1ULL << a1));
    }

    TEST(BBTester, IfACellIsSetAndThenResetBitboardIsEmpty)
    {
        BitBoard bb;
        bb.setCell(f_a, r_1);
        bb.resetCell(f_a, r_1);
        ASSERT_EQ(bb, BitBoard(EmptyBB));
    }

    TEST(BBTester, Cell_3_7_CorrespondToH4)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell(f_h, r_4), BitBoard(1ULL << h4));
    }

    TEST(BBTester, CheckCellSetByName)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell(c4), 1ULL << (f_c + r_4 * 8));
    }

    TEST(BBTester, Check1stRank)
    {
        BitBoard bb;
        bb.setCell(a1);
        bb.setCell(b1);
        bb.setCell(c1);
        bb.setCell(d1);
        bb.setCell(e1);
        bb.setCell(f1);
        bb.setCell(g1);
        ASSERT_EQ(bb.setCell(h1), RanksBB[r_1]);
    }

    TEST(BBTester, Check2ndRank)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell({a2, b2, c2, d2, e2, f2, g2, h2}), RanksBB[r_2]);
    }

    TEST(BBTester, Check3rdFile)
    {
        BitBoard bb;
        bb.setCell(c1);
        bb.setCell(c2);
        bb.setCell(c3);
        bb.setCell(c4);
        bb.setCell(c5);
        bb.setCell(c6);
        bb.setCell(c7);
        ASSERT_EQ(bb.setCell(c8), FilesBB[f_c]);
    }

    TEST(BBTester, Check7thFile)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell({g1, g2, g3, g4, g5, g6, g7, g8}), FilesBB[f_g]);
    }

    TEST(BBTester, CheckLowerLeftUpperRightDiagonal)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell({a1, b2, c3, d4, e5, f6, g7, h8}), DiagonalBB);
    }

    TEST(BBTester, CheckLowerRightUpperLeftDiagonal)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell({a8, b7, c6, d5, e4, f3, g2, h1}), AntiDiagonalBB);
    }

    TEST(BBTester, CheckCenterOfBoard)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell({d4, e4, d5, e5}), BoardCenterBB);
    }

    TEST(BBTester, CheckBothDiagonals)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell({a1, a8, b2, b7, c3, c6, d4, d5, e4, e5, f3, f6, g2, g7, h1, h8}), BothDiagonalsBB);
    }

    TEST(BBTester, CheckAllBlackCells)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell({a1, a3, a5, a7, b2, b4, b6, b8,
                              c1, c3, c5, c7, d2, d4, d6, d8,
                              e1, e3, e5, e7, f2, f4, f6, f8,
                              g1, g3, g5, g7, h2, h4, h6, h8}),
                  AllBlackCellsBB);
    }
    TEST(BBTester, CheckAllWhiteCells)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell({a2, a4, a6, a8, b1, b3, b5, b7,
                              c2, c4, c6, c8, d1, d3, d5, d7,
                              e2, e4, e6, e8, f1, f3, f5, f7,
                              g2, g4, g6, g8, h1, h3, h5, h7}),
                  AllWhiteCellsBB);
    }

    TEST(BBTester, IntersectionBetweenRank3AndFileFIsF3)
    {
        BitBoard bb;
        ASSERT_EQ(bb.setCell(f3), RanksBB[r_3] & FilesBB[f_f]);
    }

    TEST(BBTester, AddAllBlackAndWhiteCellsToObtainBitboardFull)
    {
        BitBoard bb(AllBlackCellsBB);
        ASSERT_EQ(bb | BitBoard(AllWhiteCellsBB), BitBoard(AllCellsBB));
    }

    TEST(BBTester, RemoveWhiteCellsToFullBoardToObtainBlackCellsOnly)
    {
        BitBoard bb(AllCellsBB);
        // This is a not clear way to remove cells, but it works...
        ASSERT_EQ(bb & BitBoard(~AllWhiteCellsBB), BitBoard(AllBlackCellsBB));
    }

    TEST(BBTester, TestCopyConstructor)
    {
        BitBoard bb1(AllBlackCellsBB);
        ASSERT_EQ(bb1, BitBoard(AllBlackCellsBB));
        BitBoard bb2{bb1};
        ASSERT_EQ(bb2, BitBoard(AllBlackCellsBB));
    }

    TEST(BBTester, TestAssignmentOperator)
    {
        BitBoard bb1(AllBlackCellsBB);
        ASSERT_EQ(bb1, BitBoard(AllBlackCellsBB));
        BitBoard bb2;
        bb2 = bb1;
        ASSERT_EQ(bb2, BitBoard(AllBlackCellsBB));
    }

    TEST(BBTester, TestEqualityOperatorOnEmptyBitBoards)
    {
        BitBoard bb1, bb2;
        ASSERT_EQ(bb1, bb2);
    }

    TEST(BBTester, TestEqualityOperatorOnGenericBitBoards)
    {
        BitBoard bb1(AllBlackCellsBB), bb2(AllBlackCellsBB);
        ASSERT_EQ(bb1, bb2);
    }

    TEST(BBTester, UnionOperator_TestThatUnionOfBlackAndWhiteSquaresGiveAllBoard)
    {
        BitBoard bbBlack(AllBlackCellsBB);
        BitBoard bbWhite(AllWhiteCellsBB);
        auto bbAll = bbBlack | bbWhite;
        ASSERT_EQ(bbAll, BitBoard(AllCellsBB));
    }

    TEST(BBTester, IntersectionOperator_TestThatIntersectionOfFilesDandEwithRanks4and5GiveTheBoardCenter)
    {
        BitBoard centerFiles(FilesBB[f_d] | FilesBB[f_e]);
        BitBoard centerRanks(RanksBB[r_4] | RanksBB[r_5]);
        auto bbCenter = centerFiles & centerRanks;
        ASSERT_EQ(bbCenter, BitBoard(BoardCenterBB));
    }

    TEST(BBTester, IntersectionOfDifferentSetOfRanksGiveTheEmptyBoard)
    {
        BitBoard whiteCamp(RanksBB[r_1] | RanksBB[r_2]);
        BitBoard blackCamp(RanksBB[r_7] | RanksBB[r_8]);
        BitBoard bbEmpty = whiteCamp & blackCamp;
        ASSERT_EQ(bbEmpty, BitBoard(EmptyBB));
    }

    TEST(BBTester, TestXorOperator)
    {
        BitBoard diags(BothDiagonalsBB);
        BitBoard diagsNoCenter({a1, h1, b2, g2, c3, f3, c6, f6, b7, g7, a8, h8});
        ASSERT_EQ(diags ^ diagsNoCenter, BitBoard(BoardCenterBB));
    }

    TEST(BBTester, TestXorOperatorWithBitBoardState)
    {
        BitBoard whiteCamp(RanksBB[r_1] | RanksBB[r_2]);
        BitBoard blackCamp(RanksBB[r_7] | RanksBB[r_8]);
        ASSERT_EQ(whiteCamp ^ BitBoard((RanksBB[r_7] | RanksBB[r_8])), whiteCamp | blackCamp);
    }

    TEST(BBTester, TestClearBoardOperation)
    {
        BitBoard bb(AllBlackCellsBB);
        bb.clear();
        ASSERT_EQ(bb, BitBoard(EmptyBB));
    }

    TEST(BBTester, TestCopyOperator)
    {
        BitBoard bb(AllBlackCellsBB);
        BitBoard bb2 = bb;
        ASSERT_EQ(bb2, BitBoard(AllBlackCellsBB));
    }

    TEST(BBTester, ShiftWestOfEightOrMoreFilesReturnsEmptyBoard)
    {
        BitBoard bb(AllCellsBB);
        ASSERT_EQ(bb.shiftWest(8), EmptyBB);
    }

    TEST(BBTester, ShiftWestLastFourEastFilesReturnsFourWestFiles)
    {
        BitBoard bb(FilesBB[f_e] | FilesBB[f_f] | FilesBB[f_g] | FilesBB[f_h]);
        ASSERT_EQ(bb.shiftWest(4), FilesBB[f_a] | FilesBB[f_b] | FilesBB[f_c] | FilesBB[f_d]);
    }

    TEST(BBTester, ShiftEastOfEightOrMoreFilesReturnsEmptyBoard)
    {
        BitBoard bb(AllCellsBB);
        ASSERT_EQ(bb.shiftEast(111), EmptyBB);
    }

    TEST(BBTester, ShiftEastOfOneFileBlackBoardReturnsWhiteBoardWithFirstFileEmpty)
    {
        BitBoard bb(AllBlackCellsBB);
        ASSERT_EQ(bb.shiftEast(1), (AllWhiteCellsBB & (~FilesBB[f_a])));
    }

    TEST(BBTester, ShiftNorthOfEightOrMoreFilesReturnsEmptyBoard)
    {
        BitBoard bb(AllCellsBB);
        ASSERT_EQ(bb.shiftNorth(9), EmptyBB);
    }

    TEST(BBTester, ShiftBothDiagonalNorthOfThreeRanksReturnsAFunnyRoof)
    {
        //  ...xx... = 18
        //  ...xx... = 18
        //  ..x..x.. = 24
        //  .x....x. = 42
        //  x......x = 81
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        constexpr BitBoardState FunnyRoofBB = {0x1818244281000000ULL};
        BitBoard bb(BothDiagonalsBB);
        ASSERT_EQ(bb.shiftNorth(3), FunnyRoofBB);
    }

    TEST(BBTester, ShiftRank5FileDCompoSouthOfFourRanksReturnsAFloorWithASpike)
    {
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        //  ...x.... = 08
        //  ...x.... = 08
        //  ...x.... = 08
        //  xxxxxxxx = FF
        constexpr BitBoardState FloorWithASpikeBB = {0x00000000080808FFULL};
        BitBoard bb(FilesBB[f_d] | RanksBB[r_5]);
        ASSERT_EQ(bb.shiftSouth(4), FloorWithASpikeBB);
    }
    TEST(BBTester, ShiftSouthOfMoreThan7RanksReturnsEmptyBoard)
    {
        BitBoard bb(AllCellsBB);
        ASSERT_EQ(bb.shiftSouth(8), EmptyBB);
    }

    TEST(BBTester, CellToRankAndFileConversionTests)
    {
        ASSERT_EQ(BitBoard::file(f2), f_f);
        ASSERT_EQ(BitBoard::rank(f2), r_2);
        ASSERT_EQ(BitBoard::file(a3), f_a);
        ASSERT_EQ(BitBoard::rank(a3), r_3);
        ASSERT_EQ(BitBoard::file(c7), f_c);
        ASSERT_EQ(BitBoard::rank(c7), r_7);
        ASSERT_EQ(BitBoard::file(h8), f_h);
        ASSERT_EQ(BitBoard::rank(h8), r_8);
        pair<File, Rank> coords;
        coords = BitBoard::coords(b5);
        ASSERT_EQ(coords.first, f_b);
        ASSERT_EQ(coords.second, r_5);
        coords = BitBoard::coords(e1);
        ASSERT_EQ(coords.first, f_e);
        ASSERT_EQ(coords.second, r_1);
    }

    // Evaluation of neighbours
    TEST(BBTester, NeighboursMaskOfEmptyBitBoardIsEmpty)
    {
        BitBoard bb;
        ASSERT_EQ(bb.neighbourCells(), BitBoard(EmptyBB));
    }
    TEST(BBTester, NeighboursMaskOfWhiteCellsIsAFullBoard)
    {
        BitBoard bb {AllWhiteCellsBB};
        ASSERT_EQ(bb.neighbourCells(), BitBoard(AllCellsBB));
    }
    TEST(BBTester, NeighboursMaskOfE5IsComputedCorrectly)
    {
        //  ........ = 00
        //  ........ = 00
        //  ...xxx.. = 38
        //  ...x.x.. = 28
        //  ...xxx.. = 38
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        BitBoard bb(e5);
        BitBoardState n = 0x0000382838000000ULL;
        ASSERT_EQ(bb.neighbourCells(), BitBoard(n));
    }
    TEST(BBTester, NeighboursMaskOfB2IsComputedCorrectly)
    {
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        //  xxx..... = 07
        //  x.x..... = 05
        //  xxx..... = 07
        BitBoard bb(b2);
        BitBoardState n = 0x0000000000070507ULL;
        ASSERT_EQ(bb.neighbourCells(), BitBoard(n));
    }
    TEST(BBTester, NeighboursMaskOfH6IsComputedCorrectly)
    {
        //  ........ = 00
        //  ......xx = C0
        //  ......x. = 40
        //  ......xx = C0
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        BitBoard bb(h6);
        BitBoardState n = 0x00C040C000000000ULL;
        ASSERT_EQ(bb.neighbourCells(), BitBoard(n));
    }
    TEST(BBTester, NeighboursMaskOfH1IsComputedCorrectly)
    {
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        //  ........ = 00
        //  ......xx = C0
        //  ......x. = 40
        BitBoard bb(h1);
        BitBoardState n = 0x000000000000C040ULL;
        ASSERT_EQ(bb.neighbourCells(), BitBoard(n));
    }

    TEST(BBTester, NeighboursCellsOfD4AreComputedCorrectly)
    {
        ASSERT_EQ(BitBoard::w(d4), c4);
        ASSERT_EQ(BitBoard::nw(d4), c5);
        ASSERT_EQ(BitBoard::n(d4), d5);
        ASSERT_EQ(BitBoard::ne(d4), e5);
        ASSERT_EQ(BitBoard::e(d4), e4);
        ASSERT_EQ(BitBoard::se(d4), e3);
        ASSERT_EQ(BitBoard::s(d4), d3);
        ASSERT_EQ(BitBoard::sw(d4), c3);
    }

    TEST(BBTester, NeighboursCellsOfB7AreComputedCorrectly)
    {
        ASSERT_EQ(BitBoard::w(b7), a7);
        ASSERT_EQ(BitBoard::nw(b7), a8);
        ASSERT_EQ(BitBoard::n(b7), b8);
        ASSERT_EQ(BitBoard::ne(b7), c8);
        ASSERT_EQ(BitBoard::e(b7), c7);
        ASSERT_EQ(BitBoard::se(b7), c6);
        ASSERT_EQ(BitBoard::s(b7), b6);
        ASSERT_EQ(BitBoard::sw(b7), a6);
    }

    TEST(BBTester, NeighboursCellsOfA1AreComputedCorrectly)
    {
        ASSERT_EQ(BitBoard::w(a1), InvalidCell);
        ASSERT_EQ(BitBoard::nw(a1), InvalidCell);
        ASSERT_EQ(BitBoard::n(a1), a2);
        ASSERT_EQ(BitBoard::ne(a1), b2);
        ASSERT_EQ(BitBoard::e(a1), b1);
        ASSERT_EQ(BitBoard::se(a1), InvalidCell);
        ASSERT_EQ(BitBoard::s(a1), InvalidCell);
        ASSERT_EQ(BitBoard::sw(a1), InvalidCell);
    }

    TEST(BBTester, NeighboursCellsOfH3AreComputedCorrectly)
    {
        ASSERT_EQ(BitBoard::w(h3), g3);
        ASSERT_EQ(BitBoard::nw(h3), g4);
        ASSERT_EQ(BitBoard::n(h3), h4);
        ASSERT_EQ(BitBoard::ne(h3), InvalidCell);
        ASSERT_EQ(BitBoard::e(h3), InvalidCell);
        ASSERT_EQ(BitBoard::se(h3), InvalidCell);
        ASSERT_EQ(BitBoard::s(h3), h2);
        ASSERT_EQ(BitBoard::sw(h3), g2);
    }

    TEST(BBTester, FileMasksAreComputedCorrectly)
    {
        ASSERT_EQ(BitBoard::fileMask(a2), BitBoard(FilesBB[f_a]));
        ASSERT_EQ(BitBoard::fileMask(b1), BitBoard(FilesBB[f_b]));
        ASSERT_EQ(BitBoard::fileMask(c4), BitBoard(FilesBB[f_c]));
        ASSERT_EQ(BitBoard::fileMask(d3), BitBoard(FilesBB[f_d]));
        ASSERT_EQ(BitBoard::fileMask(e6), BitBoard(FilesBB[f_e]));
        ASSERT_EQ(BitBoard::fileMask(f5), BitBoard(FilesBB[f_f]));
        ASSERT_EQ(BitBoard::fileMask(g8), BitBoard(FilesBB[f_g]));
        ASSERT_EQ(BitBoard::fileMask(h7), BitBoard(FilesBB[f_h]));
    }

    TEST(BBTester, RankMasksAreComputedCorrectly)
    {
        ASSERT_EQ(BitBoard::rankMask(a6), BitBoard(RanksBB[r_6]));
        ASSERT_EQ(BitBoard::rankMask(b7), BitBoard(RanksBB[r_7]));
        ASSERT_EQ(BitBoard::rankMask(c8), BitBoard(RanksBB[r_8]));
        ASSERT_EQ(BitBoard::rankMask(d1), BitBoard(RanksBB[r_1]));
        ASSERT_EQ(BitBoard::rankMask(e2), BitBoard(RanksBB[r_2]));
        ASSERT_EQ(BitBoard::rankMask(f3), BitBoard(RanksBB[r_3]));
        ASSERT_EQ(BitBoard::rankMask(g4), BitBoard(RanksBB[r_4]));
        ASSERT_EQ(BitBoard::rankMask(h5), BitBoard(RanksBB[r_5]));
    }

    TEST(BBTester, FileRankMasksAreComputedCorrectly)
    {
        ASSERT_EQ(BitBoard::fileRankMask(a6), BitBoard(FilesBB[f_a] | RanksBB[r_6]));
        ASSERT_EQ(BitBoard::fileRankMask(b7), BitBoard(FilesBB[f_b] | RanksBB[r_7]));
        ASSERT_EQ(BitBoard::fileRankMask(c8), BitBoard(FilesBB[f_c] | RanksBB[r_8]));
        ASSERT_EQ(BitBoard::fileRankMask(d1), BitBoard(FilesBB[f_d] | RanksBB[r_1]));
        ASSERT_EQ(BitBoard::fileRankMask(e2), BitBoard(FilesBB[f_e] | RanksBB[r_2]));
        ASSERT_EQ(BitBoard::fileRankMask(f3), BitBoard(FilesBB[f_f] | RanksBB[r_3]));
        ASSERT_EQ(BitBoard::fileRankMask(g4), BitBoard(FilesBB[f_g] | RanksBB[r_4]));
        ASSERT_EQ(BitBoard::fileRankMask(h5), BitBoard(FilesBB[f_h] | RanksBB[r_5]));
    }

    TEST(BBTester, CellToDiagonalsConversionTests)
    {
        ASSERT_EQ(BitBoard::diag(f2), d_11);
        ASSERT_EQ(BitBoard::antiDiag(f2), a_6);
        ASSERT_EQ(BitBoard::diag(a3), d_5);
        ASSERT_EQ(BitBoard::antiDiag(a3), a_2);
        ASSERT_EQ(BitBoard::diag(c7), d_3);
        ASSERT_EQ(BitBoard::antiDiag(c7), a_8);
        ASSERT_EQ(BitBoard::diag(h8), d_7);
        ASSERT_EQ(BitBoard::antiDiag(h8), a_14);
        pair<Diagonal, AntiDiagonal> diags;
        diags = BitBoard::diagonals(b5);
        ASSERT_EQ(diags.first, d_4);
        ASSERT_EQ(diags.second, a_5);
        diags = BitBoard::diagonals(e1);
        ASSERT_EQ(diags.first, d_11);
        ASSERT_EQ(diags.second, a_4);
    }

    TEST(BBTester, DiagonalMasksAreComputedCorrectly)
    {
        ASSERT_EQ(BitBoard::diagMask(a8), BitBoard(DiagsBB[d_0]));
        ASSERT_EQ(BitBoard::diagMask(b8), BitBoard(DiagsBB[d_1]));
        ASSERT_EQ(BitBoard::diagMask(a6), BitBoard(DiagsBB[d_2]));
        ASSERT_EQ(BitBoard::diagMask(d8), BitBoard(DiagsBB[d_3]));
        ASSERT_EQ(BitBoard::diagMask(d7), BitBoard(DiagsBB[d_4]));
        ASSERT_EQ(BitBoard::diagMask(e7), BitBoard(DiagsBB[d_5]));
        ASSERT_EQ(BitBoard::diagMask(c4), BitBoard(DiagsBB[d_6]));
        ASSERT_EQ(BitBoard::diagMask(f6), BitBoard(DiagsBB[d_7]));
        ASSERT_EQ(BitBoard::diagMask(b1), BitBoard(DiagsBB[d_8]));
        ASSERT_EQ(BitBoard::diagMask(h6), BitBoard(DiagsBB[d_9]));
        ASSERT_EQ(BitBoard::diagMask(d1), BitBoard(DiagsBB[d_10]));
        ASSERT_EQ(BitBoard::diagMask(e1), BitBoard(DiagsBB[d_11]));
        ASSERT_EQ(BitBoard::diagMask(g2), BitBoard(DiagsBB[d_12]));
        ASSERT_EQ(BitBoard::diagMask(g1), BitBoard(DiagsBB[d_13]));
        ASSERT_EQ(BitBoard::diagMask(h1), BitBoard(DiagsBB[d_14]));
        ASSERT_EQ(BitBoard::diagMask(f5), BitBoard({b1, c2, d3, e4, f5, g6, h7}));
    }

    TEST(BBTester, AntiDiagonalMasksAreComputedCorrectly)
    {
        ASSERT_EQ(BitBoard::antiDiagMask(a1), BitBoard(AntiDiagsBB[a_0]));
        ASSERT_EQ(BitBoard::antiDiagMask(a2), BitBoard(AntiDiagsBB[a_1]));
        ASSERT_EQ(BitBoard::antiDiagMask(c1), BitBoard(AntiDiagsBB[a_2]));
        ASSERT_EQ(BitBoard::antiDiagMask(b3), BitBoard(AntiDiagsBB[a_3]));
        ASSERT_EQ(BitBoard::antiDiagMask(b4), BitBoard(AntiDiagsBB[a_4]));
        ASSERT_EQ(BitBoard::antiDiagMask(d3), BitBoard(AntiDiagsBB[a_5]));
        ASSERT_EQ(BitBoard::antiDiagMask(f2), BitBoard(AntiDiagsBB[a_6]));
        ASSERT_EQ(BitBoard::antiDiagMask(a8), BitBoard(AntiDiagsBB[a_7]));
        ASSERT_EQ(BitBoard::antiDiagMask(e5), BitBoard(AntiDiagsBB[a_8]));
        ASSERT_EQ(BitBoard::antiDiagMask(h3), BitBoard(AntiDiagsBB[a_9]));
        ASSERT_EQ(BitBoard::antiDiagMask(g5), BitBoard(AntiDiagsBB[a_10]));
        ASSERT_EQ(BitBoard::antiDiagMask(e8), BitBoard(AntiDiagsBB[a_11]));
        ASSERT_EQ(BitBoard::antiDiagMask(g7), BitBoard(AntiDiagsBB[a_12]));
        ASSERT_EQ(BitBoard::antiDiagMask(g8), BitBoard(AntiDiagsBB[a_13]));
        ASSERT_EQ(BitBoard::antiDiagMask(h8), BitBoard(AntiDiagsBB[a_14]));
        ASSERT_EQ(BitBoard::antiDiagMask(f5), BitBoard({h3, g4, f5, e6, d7, c8}));
    }

    TEST(BBTester, DiagonalAntiDiagonalMasksAreComputedCorrectly)
    {
        ASSERT_EQ(BitBoard::diagonalsMask(a6), BitBoard(DiagsBB[d_2] | AntiDiagsBB[a_5]));
        ASSERT_EQ(BitBoard::diagonalsMask(a6), BitBoard({f1, e2, d3, c4, b5, a6, b7, c8}));
        ASSERT_EQ(BitBoard::diagonalsMask(b7), BitBoard(DiagsBB[d_2] | AntiDiagsBB[a_7]));
        ASSERT_EQ(BitBoard::diagonalsMask(c8), BitBoard(DiagsBB[d_2] | AntiDiagsBB[a_9]));
        ASSERT_EQ(BitBoard::diagonalsMask(d1), BitBoard(DiagsBB[d_10] | AntiDiagsBB[a_3]));
        ASSERT_EQ(BitBoard::diagonalsMask(e2), BitBoard(DiagsBB[d_10] | AntiDiagsBB[a_5]));
        ASSERT_EQ(BitBoard::diagonalsMask(f3), BitBoard(DiagsBB[d_10] | AntiDiagsBB[a_7]));
        ASSERT_EQ(BitBoard::diagonalsMask(g4), BitBoard(DiagsBB[d_10] | AntiDiagsBB[a_9]));
        ASSERT_EQ(BitBoard::diagonalsMask(h5), BitBoard(DiagsBB[d_10] | AntiDiagsBB[a_11]));
    }

    TEST(BBTester, QueenMasksAreComputedCorrectly)
    {
        ASSERT_EQ(BitBoard::queenMask(a8), BitBoard(FilesBB[f_a] | RanksBB[r_8] | DiagsBB[d_0]  | AntiDiagsBB[a_7]));
        ASSERT_EQ(BitBoard::queenMask(b8), BitBoard(FilesBB[f_b] | RanksBB[r_8] | DiagsBB[d_1]  | AntiDiagsBB[a_8]));
        ASSERT_EQ(BitBoard::queenMask(a6), BitBoard(FilesBB[f_a] | RanksBB[r_6] | DiagsBB[d_2]  | AntiDiagsBB[a_5]));
        ASSERT_EQ(BitBoard::queenMask(d8), BitBoard(FilesBB[f_d] | RanksBB[r_8] | DiagsBB[d_3]  | AntiDiagsBB[a_10]));
        ASSERT_EQ(BitBoard::queenMask(d7), BitBoard(FilesBB[f_d] | RanksBB[r_7] | DiagsBB[d_4]  | AntiDiagsBB[a_9]));
        ASSERT_EQ(BitBoard::queenMask(e7), BitBoard(FilesBB[f_e] | RanksBB[r_7] | DiagsBB[d_5]  | AntiDiagsBB[a_10]));
        ASSERT_EQ(BitBoard::queenMask(c4), BitBoard(FilesBB[f_c] | RanksBB[r_4] | DiagsBB[d_6]  | AntiDiagsBB[a_5]));
        ASSERT_EQ(BitBoard::queenMask(f6), BitBoard(FilesBB[f_f] | RanksBB[r_6] | DiagsBB[d_7]  | AntiDiagsBB[a_10]));
        ASSERT_EQ(BitBoard::queenMask(b1), BitBoard(FilesBB[f_b] | RanksBB[r_1] | DiagsBB[d_8]  | AntiDiagsBB[a_1]));
        ASSERT_EQ(BitBoard::queenMask(h6), BitBoard(FilesBB[f_h] | RanksBB[r_6] | DiagsBB[d_9]  | AntiDiagsBB[a_12]));
        ASSERT_EQ(BitBoard::queenMask(d1), BitBoard(FilesBB[f_d] | RanksBB[r_1] | DiagsBB[d_10] | AntiDiagsBB[a_3]));
        ASSERT_EQ(BitBoard::queenMask(e1), BitBoard(FilesBB[f_e] | RanksBB[r_1] | DiagsBB[d_11] | AntiDiagsBB[a_4]));
        ASSERT_EQ(BitBoard::queenMask(g2), BitBoard(FilesBB[f_g] | RanksBB[r_2] | DiagsBB[d_12] | AntiDiagsBB[a_7]));
        ASSERT_EQ(BitBoard::queenMask(g1), BitBoard(FilesBB[f_g] | RanksBB[r_1] | DiagsBB[d_13] | AntiDiagsBB[a_6]));
        ASSERT_EQ(BitBoard::queenMask(h1), BitBoard(FilesBB[f_h] | RanksBB[r_1] | DiagsBB[d_14] | AntiDiagsBB[a_7]));
        ASSERT_EQ(BitBoard::queenMask(a1), BitBoard(FilesBB[f_a] | RanksBB[r_1] | DiagsBB[d_7]  | AntiDiagsBB[a_0]));
        ASSERT_EQ(BitBoard::queenMask(a2), BitBoard(FilesBB[f_a] | RanksBB[r_2] | DiagsBB[d_6]  | AntiDiagsBB[a_1]));
        ASSERT_EQ(BitBoard::queenMask(c1), BitBoard(FilesBB[f_c] | RanksBB[r_1] | DiagsBB[d_9]  | AntiDiagsBB[a_2]));
        ASSERT_EQ(BitBoard::queenMask(b3), BitBoard(FilesBB[f_b] | RanksBB[r_3] | DiagsBB[d_6]  | AntiDiagsBB[a_3]));
        ASSERT_EQ(BitBoard::queenMask(b4), BitBoard(FilesBB[f_b] | RanksBB[r_4] | DiagsBB[d_5]  | AntiDiagsBB[a_4]));
        ASSERT_EQ(BitBoard::queenMask(d3), BitBoard(FilesBB[f_d] | RanksBB[r_3] | DiagsBB[d_8]  | AntiDiagsBB[a_5]));
        ASSERT_EQ(BitBoard::queenMask(f2), BitBoard(FilesBB[f_f] | RanksBB[r_2] | DiagsBB[d_11]  | AntiDiagsBB[a_6]));
        ASSERT_EQ(BitBoard::queenMask(a8), BitBoard(FilesBB[f_a] | RanksBB[r_8] | DiagsBB[d_14]  | AntiDiagsBB[a_7]));
        ASSERT_EQ(BitBoard::queenMask(e5), BitBoard(FilesBB[f_e] | RanksBB[r_5] | DiagsBB[d_7]  | AntiDiagsBB[a_8]));
        ASSERT_EQ(BitBoard::queenMask(h3), BitBoard(FilesBB[f_h] | RanksBB[r_3] | DiagsBB[d_12]  | AntiDiagsBB[a_9]));
        ASSERT_EQ(BitBoard::queenMask(g5), BitBoard(FilesBB[f_g] | RanksBB[r_5] | DiagsBB[d_9] | AntiDiagsBB[a_10]));
        ASSERT_EQ(BitBoard::queenMask(e8), BitBoard(FilesBB[f_e] | RanksBB[r_8] | DiagsBB[d_4] | AntiDiagsBB[a_11]));
        ASSERT_EQ(BitBoard::queenMask(g7), BitBoard(FilesBB[f_g] | RanksBB[r_7] | DiagsBB[d_7] | AntiDiagsBB[a_12]));
        ASSERT_EQ(BitBoard::queenMask(g8), BitBoard(FilesBB[f_g] | RanksBB[r_8] | DiagsBB[d_6] | AntiDiagsBB[a_13]));
        ASSERT_EQ(BitBoard::queenMask(h8), BitBoard(FilesBB[f_h] | RanksBB[r_8] | DiagsBB[d_7] | AntiDiagsBB[a_14]));
    }

    // population count calculation tests
    TEST(BBTester, OnlyA1PopulationCountIs1)
    {
        BitBoard bb {a1};
        ASSERT_EQ(bb.popCount(), 1);
    }
    TEST(BBTester, BoardCenterPopulationCountIs4)
    {
        BitBoard bb(BoardCenterBB);
        ASSERT_EQ(bb.popCount(), 4);
    }
    TEST(BBTester, SingleRankPopulationCountIs8)
    {
        BitBoard bb(RanksBB[r_3]);
        ASSERT_EQ(bb.popCount(), 8);
    }
    TEST(BBTester, SingleFilePopulationCountIs8)
    {
        BitBoard bb(FilesBB[f_h]);
        ASSERT_EQ(bb.popCount(), 8);
    }
    TEST(BBTester, BoardDiagonalPopulationCountIs8)
    {
        BitBoard bb(DiagsBB[d_7]);
        ASSERT_EQ(bb.popCount(), 8);
    }
    TEST(BBTester, QueenMaskOfC3PopulationCountIs24)
    {
        BitBoard bb(BitBoard::queenMask(c3));
        ASSERT_EQ(bb.popCount(), 26);
    }
    TEST(BBTester, WhiteCellsPopulationCountIs32)
    {
        BitBoard bb(AllWhiteCellsBB);
        ASSERT_EQ(bb.popCount(), 32);
    }
    TEST(BBTester, FullBoardPopulationCountIs64)
    {
        BitBoard bb(AllCellsBB);
        ASSERT_EQ(bb.popCount(), 64);
    }

    // Check funtions tests
    TEST(BBTester, IfCellE3IsActiveThereAreActiveCellsInRank3AndFileE)
    {
        BitBoard bb {e3};
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_3]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_e]));

        ASSERT_FALSE(bb.activeCellsInMask(RanksBB[r_1]));
        ASSERT_FALSE(bb.activeCellsInMask(RanksBB[r_2]));
        ASSERT_FALSE(bb.activeCellsInMask(RanksBB[r_4]));
        ASSERT_FALSE(bb.activeCellsInMask(RanksBB[r_5]));
        ASSERT_FALSE(bb.activeCellsInMask(RanksBB[r_6]));
        ASSERT_FALSE(bb.activeCellsInMask(RanksBB[r_7]));
        ASSERT_FALSE(bb.activeCellsInMask(RanksBB[r_8]));
        ASSERT_FALSE(bb.activeCellsInMask(FilesBB[f_a]));
        ASSERT_FALSE(bb.activeCellsInMask(FilesBB[f_b]));
        ASSERT_FALSE(bb.activeCellsInMask(FilesBB[f_c]));
        ASSERT_FALSE(bb.activeCellsInMask(FilesBB[f_d]));
        ASSERT_FALSE(bb.activeCellsInMask(FilesBB[f_f]));
        ASSERT_FALSE(bb.activeCellsInMask(FilesBB[f_g]));
        ASSERT_FALSE(bb.activeCellsInMask(FilesBB[f_h]));
    }

    TEST(BBTester, DiagonalTouchAllRanskAndFiles)
    {
        BitBoard bb {DiagonalBB};
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_1]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_2]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_3]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_4]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_5]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_6]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_7]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_8]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_a]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_b]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_c]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_d]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_e]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_f]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_g]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_h]));
    }

    TEST(BBTester, DiagonalsMaskOfA4DoesNotTouchLastThreeFiles)
    {
        BitBoard bb {BitBoard::diagonalsMask(a4)};
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_1]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_2]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_3]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_4]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_5]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_6]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_7]));
        ASSERT_TRUE(bb.activeCellsInMask(RanksBB[r_8]));

        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_a]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_b]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_c]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_d]));
        ASSERT_TRUE(bb.activeCellsInMask(FilesBB[f_e]));
        ASSERT_FALSE(bb.activeCellsInMask(FilesBB[f_f]));
        ASSERT_FALSE(bb.activeCellsInMask(FilesBB[f_g]));
        ASSERT_FALSE(bb.activeCellsInMask(FilesBB[f_h]));
    }

    // Tests for calcCellAfterSteps methods
    TEST(BBTester, calcCellAfterStepsTest)
    {
        ASSERT_EQ(BitBoard::calcCellAfterSteps(a1,  5,  0), a6);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(a1,  0,  5), f1);

        ASSERT_EQ(BitBoard::calcCellAfterSteps(d4,  2,  1), e6);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(d4,  1,  2), f5);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(d4, -1,  2), f3);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(d4, -2,  1), e2);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(d4, -2, -1), c2);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(d4, -1, -2), b3);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(d4,  1, -2), b5);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(d4,  2, -1), c6);

        ASSERT_EQ(BitBoard::calcCellAfterSteps(c2,  6, -2), a8);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(c2,  1, -3), InvalidCell);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(b7,  1,  3), e8);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(b7,  2, -1), InvalidCell);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(f5,  3,  2), h8);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(f5,  1,  3), InvalidCell);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(d2,  -1,  -3), a1);
        ASSERT_EQ(BitBoard::calcCellAfterSteps(d2,  -2,  -1), InvalidCell);
    }

    // Evaluations of file/rank/diagonals evaluation
    TEST(BBTester, DiagonalsBitBoardOfEmptyBoardReturnsEmpty)
    {
        BitBoard bb;
        ASSERT_EQ(bb.diagonalsCells(), BitBoard(EmptyBB));
    }
    TEST(BBTester, DiagonalsBitBoardOfFullBoardReturnsFullBoard)
    {
        BitBoard bb {AllCellsBB};
        ASSERT_EQ(bb.diagonalsCells(), BitBoard(AllCellsBB));
    }
    TEST(BBTester, evalDiagonalOfActiveCellInE4)
    {
        BitBoard bb {e4};
        ASSERT_EQ(bb.diagonalsCells(), BitBoard({b1, h1, c2, g2, d3, f3, d5, f5, c6, g6, b7, h7, a8}));
    }
    TEST(BBTester, evalFileRankOfActiveCellInE4)
    {
        BitBoard bb {e4};
        ASSERT_EQ(bb.fileRankCells(), BitBoard({e1, e2, e3, a4, b4, c4, d4, f4, g4, h4, e5, e6, e7, e8}));
    }
    TEST(BBTester, evalFileRankDiagonalsOfActiveCellInE4)
    {
        BitBoard bb {e4};
        ASSERT_EQ(bb.fileRankDiagonalsCells(), BitBoard({b1, h1, c2, g2, d3, f3, d5, f5, c6, g6, b7, h7, a8,
                                                e1, e2, e3, a4, b4, c4, d4, f4, g4, h4, e5, e6, e7, e8}));
    }
    TEST(BBTester, evalDiagonalsOfActiveCellInC3)
    {
        BitBoard bb {c3};
        ASSERT_EQ(bb.diagonalsCells(), BitBoard({a1, e1, b2, d2, b4, d4, a5, e5, f6, g7, h8}));
    }
    TEST(BBTester, evalDiagonalsOfActiveCellInF5)
    {
        BitBoard bb {f5};
        ASSERT_EQ(bb.diagonalsCells(), BitBoard({b1, c2, d3, h3, e4, g4, e6, g6, d7, h7, c8}));
    }
    TEST(BBTester, evalDiagonalsOfActiveCellsInC3AndF5)
    {
        BitBoard bb ({c3, f5});
        ASSERT_EQ(bb.diagonalsCells(), BitBoard({a1, b1, e1, b2, c2, d2, d3, h3, b4, d4, e4, g4,
                                                 a5, e5, e6, f6, g6, d7, g7, h7, c8, h8}));
    }
    TEST(BBTester, evalFilesRanksOfActiveCellsInC3AndE5)
    {
        BitBoard bb ({c3, f5});
        ASSERT_EQ(bb.fileRankCells(), BitBoard({c1, f1, c2, f2, a3, b3, d3, e3, f3, g3, h3, c4, f4,
                                                a5, b5, c5, d5, e5, g5, h5, c6, f6, c7, f7, c8, f8}));
    }
    TEST(BBTester, evalFilesRanksDiagonalsOfActiveCellInC3AndE5)
    {
        BitBoard bb ({c3, f5});
        ASSERT_EQ(bb.fileRankDiagonalsCells(), BitBoard({a1, b1, e1, b2, c2, d2, d3, h3, b4, d4, e4, g4,
                                                a5, e5, e6, f6, g6, d7, g7, h7, c8, h8, c1, f1,
                                                f2, a3, b3, e3, f3, g3, c4, f4, b5, c5, d5, g5,
                                                h5, c6, c7, f7, f8}));
    }


} // namespace cSzd
