#pragma once

//#define uchar char

namespace chess
{
	namespace Pieces
	{
		
		static const uchar None		= 0;		// 0
		static const uchar King		= 0b001;	// 1
		static const uchar Pawn		= 0b010;	// 2
		static const uchar Knight	= 0b011;	// 3
		static const uchar Bishop	= 0b100;	// 4
		static const uchar Rook		= 0b101;	// 5
		static const uchar Queen	= 0b110;	// 6
		
		/*
		enum class Type : uchar {
			None,		// 0b000 0
			King,		// 0b001 1
			Knight,		// 0b010 2
			Pawn,		// 0b011 3
			Bishop,		// 0b100 4
			Rook,		// 0b101 5
			Queen		// 0b110 6
		};
		*/

		static const uchar White	= 0b01000;	// 8
		static const uchar Black	= 0b10000;	// 16
		
		static const uchar colorIdentity = 0b11000;
		static const uchar typeIdentity	= 0b00111;


		static inline bool IsColor(uchar piece, uchar color) {
			return (piece & colorIdentity) == color;
		}

		static inline bool IsWhite(uchar piece) {
			return IsColor(piece, White);
		}

		static inline bool IsType(uchar piece, uchar type) {
			return (piece & typeIdentity) == type;
		}

		static inline bool IsSlidingPiece(uchar piece) {
			return piece & 0b100;
		}

		static inline uchar GetColor(uchar piece) {
			return piece & colorIdentity;
		}

		static inline uchar GetType(uchar piece) {
			return piece & typeIdentity;
		}
	}

	
}