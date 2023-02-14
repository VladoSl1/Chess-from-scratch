#pragma once

/*
FEN = Forsyth–Edwards Notation
*/

#include "Pieces.h"
#include "Orientation.h"

#include "std_extension.h"

#include <map>


namespace chess
{
	struct PositionInfo
	{
		uchar squares[64];
		bool whiteOnMove			= 0;
		uchar castling				= 0;	// 1.bit - W King, 2. - W Queen, 3. - B King, 4. - W Queen
		uchar enPassant				= 0;	// position
		int halfMoveCounter			= 0;
		int fullMoveCounter			= 0;

		PositionInfo() {
			std::fill_n(squares, 64, Pieces::None);
		}
	};

	namespace fenutil
	{
		static const std::string StartingPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

		static std::map<uchar, char> CharFromType = {
			{ Pieces::King,		'K'},
			{ Pieces::Knight,	'N'},
			{ Pieces::Pawn,		'P'},
			{ Pieces::Bishop,	'B'},
			{ Pieces::Rook,		'R'},
			{ Pieces::Queen,	'Q'},
		};

		static std::map<char, uchar> TypeFromChar = {
			{ 'K', Pieces::King },
			{ 'N', Pieces::Knight },
			{ 'P', Pieces::Pawn },
			{ 'B', Pieces::Bishop },
			{ 'R', Pieces::Rook },
			{ 'Q', Pieces::Queen },
		};

		static PositionInfo* LoadPosition(const std::string& pos)
		{
			PositionInfo* posInfo = new PositionInfo;
			std::vector<std::string> fenParts = splitStr(pos);

			posInfo->whiteOnMove		= fenParts[1] == "w";
			posInfo->enPassant			= fenParts[3][0] != '-' ? ((fenParts[3][0] - 97) << 3) | (fenParts[3][1] - '0') : -1;
			posInfo->halfMoveCounter	= std::stoi(fenParts[4]);
			posInfo->fullMoveCounter	= std::stoi(fenParts[5]);

			uchar piece;
			int file = 0, rank = 7;
			for (char symbol : fenParts[0])
			{
				if (symbol == '/')				// new line
				{
					rank--;
					file = 0;
				}

				else if (isdigit(symbol))		// number of empty squares
					file += symbol - '0';
					
				else							// piece
				{
					piece = 0;
					piece |= islower(symbol) ? Pieces::Black : Pieces::White;
					piece |= TypeFromChar[toupper(symbol)];
					posInfo->squares[8 * rank + file] = piece;
					file++;
				}
			}

			for (char symbol : fenParts[2])
			{
				switch (symbol)
				{
				case 'K':
					posInfo->castling |= 0b1000;
					break;
				case 'Q':
					posInfo->castling |= 0b0100;
					break;
				case 'k':
					posInfo->castling |= 0b0010;
					break;
				case 'q':
					posInfo->castling |= 0b0001;
					break;
				}
			}

		return posInfo;
		}


		static std::string SavePosition(const PositionInfo* const posInfo)
		{
			std::string res = "";

			/* First: Piece placement */
			uchar piece, symbol, spaceCounter = 0;
			for (int rank = 7; rank >= 0; rank--)
			{
				for (int file = 0; file < 8; file++)
				{
					piece = posInfo->squares[orient::PosFromCoords(rank, file)];

					if (piece == Pieces::None)
						spaceCounter++;

					else
					{
						if (spaceCounter)
							res += std::to_string((int)spaceCounter);
						spaceCounter = 0;

						symbol = CharFromType[Pieces::GetType(piece)];
						res += Pieces::IsColor(piece, Pieces::White) ? symbol : tolower(symbol);
					}
				}

				if (spaceCounter)
					res += std::to_string((int)spaceCounter);
				spaceCounter = 0;

				res += "/";
			}
			res.pop_back();

			/* Second: Active color */
			res += posInfo->whiteOnMove ? " w" : " b";

			/* Third: Castling availability */
			res += " ";
			if (posInfo->castling & 0b1000)
				res += "K";
			if (posInfo->castling & 0b0100)
				res += "Q";
			if (posInfo->castling & 0b0010)
				res += "k";
			if (posInfo->castling & 0b0001)
				res += "q";
			if (res[res.length() - 1] == ' ')
				res += "-";

			/* Fourth: En pasant */
			res += " ";
			if (posInfo->enPassant != -1)
			{
				res += ((posInfo->enPassant & 0b111000) >> 3) + 97;
				res += std::to_string((int)(posInfo->enPassant & 0b000111));
			}
			else
				res += "-";

			/* Fifth: Halfmove count */
			res += " " + std::to_string(posInfo->halfMoveCounter) + " ";

			/* Sixth: Fullmove count */
			res += std::to_string(posInfo->fullMoveCounter);

			return res;
		}
	}
}