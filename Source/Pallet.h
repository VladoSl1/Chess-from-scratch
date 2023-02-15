#pragma once

#include "Mathematics/vec4.h"
#include "Graphics/Texture2D.h"
#include "Pieces.h"

namespace chess
{
	namespace pallet
	{
		struct SquarePallet
		{
			math::vec3<> whiteSquareColor;
			math::vec3<> blackSquareColor;
			math::vec3<> whiteSquareAttacked;
			math::vec3<> blackSquareAttacked;

			SquarePallet(math::vec3<> white, math::vec3<> black,
						 math::vec3<> whiteAttacked, math::vec3<> blackAttacked)
				: whiteSquareColor(white / 255.0f), blackSquareColor(black / 255.0f),
				  whiteSquareAttacked(whiteAttacked / 255.0f), blackSquareAttacked(blackAttacked / 255.0f) { }
		};

		struct PiecePallet
		{
			graphics::Texture2D* pieceTextures[12];

			graphics::Texture2D* GetTexture(uchar piece)
			{
				int colorShift = Pieces::IsColor(piece, Pieces::Black) * 6;

				switch (Pieces::GetType(piece))
				{
				case Pieces::King:
					return pieceTextures[0 + colorShift];
				case Pieces::Queen:
					return pieceTextures[1 + colorShift];
				case Pieces::Bishop:
					return pieceTextures[2 + colorShift];
				case Pieces::Knight:
					return pieceTextures[3 + colorShift];
				case Pieces::Rook:
					return pieceTextures[4 + colorShift];
				case Pieces::Pawn:
					return pieceTextures[5 + colorShift];
				}

				return nullptr;
			}

			PiecePallet(const std::string* paths)
			{
				for (uchar i = 0; i < 12; ++i)
					pieceTextures[i] = new graphics::Texture2D(paths[i].c_str());
			}

			~PiecePallet()
			{
				for (uchar i = 0; i < 12; ++i)
					delete pieceTextures[i];
			}

		};
	}	
}

