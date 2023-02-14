#pragma once

#include "fenutil.h"
#include "Pieces.h"

#include "Mathematics.h"

#include <functional>

/*
* pinnedPieces should be set<int> and contain only position of pieces
* look-up table for pinnedPieces
* 
* 
*/


namespace chess
{
	struct PinnedPiece
	{
		uchar pos;
		uchar atkDir;		// direction from attacker perspective
		uchar atkPos;

		PinnedPiece(uchar pos, uchar atkDir, uchar atkPos) : pos(pos), atkDir(atkDir), atkPos(atkPos) { }
	};

	namespace movegen
	{
		enum class Dirs : uchar { Up, Right, Down, Left };


		static std::set<PinnedPiece> pinnedPieces;

		inline static void add_pinned_piece(int atkPos, Dirs atkDir, int piecePos)
		{
			if (piecePos != 0)
				pinnedPieces.insert(PinnedPiece(piecePos, (uchar)atkDir, atkPos));
		}

		inline static bool valid_pos(int pos) {
			return pos >= 0 && pos <= 63;
		}

		/* For: Pawn, King, Knight */
		inline static bool small_boundary_check(int pos, int move)
		{
			int new_pos = pos + move;
			return valid_pos(new_pos) && abs(new_pos % 8 - pos % 8) <= 2;
		}

		inline static bool not_ally(int pos, int new_pos, const uchar* const squares) {
			return Pieces::GetColor(squares[pos]) != Pieces::GetColor(squares[new_pos]);
		}
		
		inline static bool is_enemy(int pos, int new_pos, const uchar* const squares) {
			return not_ally(pos, new_pos, squares) && squares[new_pos] != Pieces::None;
		}

		/* Check if 'move' is possible for King, Knight or Pawn */
		inline static bool possible_move(int pos, int move, const uchar* const squares, bool attackMove) {
			return small_boundary_check(pos, move) && ((!attackMove && not_ally(pos, pos + move, squares)) || attackMove);
		}

		static std::set<int> PawnMoves(int pos, const PositionInfo* const posInfo, bool attackMove = false)
		{
			std::set<int> res;

			int c_orient = Pieces::IsColor(posInfo->squares[pos], Pieces::White) ? 1 : -1;

			/* Move forward */
			if (posInfo->squares[pos + 8 * c_orient] == Pieces::None && !attackMove)
			{
				res.insert(pos + 8 * c_orient);
				if (posInfo->squares[pos + 16 * c_orient] == Pieces::None && pos / 8 == int(3.5f - 2.5f * c_orient))
					res.insert(pos + 16 * c_orient);
			}

			/* En passant */
			if (posInfo->enPassant != -1)
			{
				if (possible_move(pos, 9 * c_orient, posInfo->squares, false) && posInfo->enPassant == pos + 9 * c_orient)
					res.insert(pos + 9 * c_orient);
				if (possible_move(pos, 7 * c_orient, posInfo->squares, false) && posInfo->enPassant == pos + 7 * c_orient)
					res.insert(pos + 7 * c_orient);
			}

			/* Attack */
			if (small_boundary_check(pos, 9 * c_orient) && (is_enemy(pos, pos + 9 * c_orient, posInfo->squares) || attackMove))
				res.insert(pos + 9 * c_orient);
			if (small_boundary_check(pos, 7 * c_orient) && (is_enemy(pos, pos + 7 * c_orient, posInfo->squares) || attackMove))
				res.insert(pos + 7 * c_orient);

			return res;
		}

		static std::set<int> KingMoves(int pos, const PositionInfo* const posInfo, bool attackMove = false)
		{
			std::set<int> res;
			bool w = Pieces::IsColor(posInfo->squares[pos], Pieces::White);
			
			for (int move : { -1, -7, -8, -9, 1, 7, 8, 9 })
				if (possible_move(pos, move, posInfo->squares, attackMove))
					res.insert(pos + move);

			/* castling */
			if ((w && posInfo->castling & 0b1000) || (!w && posInfo->castling & 0b0010))
				if (!posInfo->squares[pos + 1] && !posInfo->squares[pos + 2])
					res.insert(pos + 2);

			if ((w && posInfo->castling & 0b0100) || (!w && posInfo->castling & 0b0001))
				if (!posInfo->squares[pos - 1] && !posInfo->squares[pos - 2] && !posInfo->squares[pos - 3])
					res.insert(pos - 2);
			
			return res;
		}

		/* returns pinned piece (if none then 0) */
		static int SlidingPieceMoves(int start, int max_end, int step, const uchar* const squares, std::set<int>* dst, bool attackMove = false)
		{
			int orient = step < 0 ? -1 : 1;
			int pinnedPiece = -1;

			for (int new_pos = start + step; new_pos * orient <= max_end * orient; new_pos += step)
			{
				if (not_ally(start, new_pos, squares))
				{
					dst->insert(new_pos);
					/*if (squares[new_pos] != Pieces::None && !attackMove)
						break;
					*/
					if (squares[new_pos] != Pieces::None)
					{
						if (!attackMove)
							break;
						
						if (Pieces::IsType(squares[new_pos], Pieces::King))
						{
							if (pinnedPiece != -1)
							{
								return pinnedPiece;
							}
						}
						else if (pinnedPiece == -1)
							pinnedPiece = new_pos;
						else
							break;
					}
					
					/*
					dst->insert(new_pos);
					if (squares[new_pos] == Pieces::None || attackMove)
						continue;
					/**/
				}
				else {
					if (attackMove)
						dst->insert(new_pos);
					break;
				}
			}

			return 0;
		}

		static std::set<int> RookMoves(int pos, const PositionInfo* const posInfo, bool attackMove = false)
		{
			std::set<int> res;

			add_pinned_piece(pos, Dirs::Up, SlidingPieceMoves(pos, 63, 8, posInfo->squares, &res, attackMove));						// up
			add_pinned_piece(pos, Dirs::Down, SlidingPieceMoves(pos, 0, -8, posInfo->squares, &res, attackMove));					// down
			add_pinned_piece(pos, Dirs::Left, SlidingPieceMoves(pos, pos - pos % 8, -1, posInfo->squares, &res, attackMove));		// left
			add_pinned_piece(pos, Dirs::Right, SlidingPieceMoves(pos, pos + 7 - pos % 8, 1, posInfo->squares, &res, attackMove));	// right

			return res;
		}

		static std::set<int> KnightMoves(int pos, const PositionInfo* const posInfo, bool attackMove = false)
		{
			std::set<int> res;
			
			for (int move : { -6, -10, -15, -17, 6, 10, 15, 17 })
				if (possible_move(pos, move, posInfo->squares, attackMove))
					res.insert(pos + move);
			
			return res;
		}

		static std::set<int> BishopMoves(int pos, const PositionInfo* const posInfo, bool attackMove = false)
		{
			std::set<int> res;
			
			add_pinned_piece(pos, Dirs::Up, SlidingPieceMoves(pos, math::min(63, pos + 8 * (7 - pos % 8) + 7), 9, posInfo->squares, &res, attackMove));		// up-right
			add_pinned_piece(pos, Dirs::Right, SlidingPieceMoves(pos, math::max(0, pos - 8 * (7 - pos % 8)), -7, posInfo->squares, &res, attackMove));		// down-right
			add_pinned_piece(pos, Dirs::Down, SlidingPieceMoves(pos, math::max(0, pos - 8 * (pos % 8) - 7), -9, posInfo->squares, &res, attackMove));		// down-left
			add_pinned_piece(pos, Dirs::Left, SlidingPieceMoves(pos, math::min(63, pos + 8 * (pos % 8)), 7, posInfo->squares, &res, attackMove));			// up-left

			return res;
		}

		static std::set<int> QueenMoves(int pos, const PositionInfo* const posInfo, bool attackMove = false)
		{
			std::set<int> rookMoves		= RookMoves(pos, posInfo, attackMove);
			std::set<int> bishopMoves	= BishopMoves(pos, posInfo, attackMove);

			bishopMoves.merge(rookMoves);

			return bishopMoves;
		}

		/* attackMove: false if getting moves for movement */
		static std::set<int> GenerateMoves(int pos, const PositionInfo* const posInfo, bool attackMove = false)
		{
			switch (Pieces::GetType(posInfo->squares[pos]))
			{
			case Pieces::Bishop:
				return BishopMoves(pos, posInfo, attackMove);
			case Pieces::Knight:
				return KnightMoves(pos, posInfo, attackMove);
			case Pieces::King:
				return KingMoves(pos, posInfo, attackMove);
			case Pieces::Pawn:
				return PawnMoves(pos, posInfo, attackMove);
			case Pieces::Queen:
				return QueenMoves(pos, posInfo, attackMove);
			case Pieces::Rook:
				return RookMoves(pos, posInfo, attackMove);
			}
		}
	};
}