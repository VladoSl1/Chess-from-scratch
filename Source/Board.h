#pragma once

#include "Graphics/SpriteRenderer.h"
#include "Graphics/GameObject.h"

#include "Pallet.h"
#include "fenutil.h"

/*
	- change rendering to batch rendering

*/


#include <vector>
#include <set>
#include <unordered_map>

namespace chess
{

	enum class Side {
		white,
		black
	};

	struct PieceSet
	{
		std::set<char> pieces;
		char kingPos = -1;
	};


	class Board
	{
		/* square: attacking pieces */
		using AttackMap = std::unordered_map<int, std::set<int>>;

	public:
		PositionInfo* posInfo;
		graphics::GameObject g_pieces[64];

		Board(Side orientation, pallet::SquarePallet* boardColorPallet, pallet::PiecePallet* piecePallet);
		~Board();

		void LoadPosition(const std::string& fen_position);

		void Draw();

		void StartPieceDrag(int pos, const math::vec2<>& coords);
		void EndPieceDrag(int pos);
		inline void DragPiece(const math::vec2<>& coords) {
			g_pieces[draggedPiece].position = coords - dragShift;
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      

		inline void SwitchOrientation() {
			orient = orient == Side::white ? Side::black : Side::white;
		}

	private:
		Side					orient;
		pallet::SquarePallet*	boardColors;
		pallet::PiecePallet*	pieceTextures;

		//graphics::GameObject whiteSquare;
		//graphics::GameObject blackSquare;
		graphics::Texture2D baseSquareTexture;
		graphics::GameObject g_squares[64];
		

		inline PieceSet* GetPieceSet(bool color_w) {
			return color_w ? &white : &black;
		}
		inline PieceSet* const GetPieceSet(bool color_w) const {
			return color_w ? const_cast<PieceSet*>(&white) : const_cast<PieceSet*>(&black);
		}

		//graphics::GameObject piece;

		std::set<int> possibleMoves;
		AttackMap attackedSquares;

		void MovePiece(int start, int end);
		bool PawnEnPassant(int start, int end);
		void RookCastling(int start, int end);
		void KingCastling(int start, int end);

		void UpdateKingState(bool color_w);
		std::set<int> counterMoves;
		std::set<int> pinnedPieces;
		/* -2: two attacking, -1: no attacking, >=0: position of attacking */
		int kingAttacker = -1;



		/* Returns squares attacked by color 'w' */
		AttackMap GetAttackedSquares(bool color_w) const;
		
		PieceSet white;
		PieceSet black;

		void InitGraphics(pallet::SquarePallet* colorPallet);

		graphics::SpriteRenderer* spriteRenderer;

		math::vec2<> dragShift;
		int draggedPiece = -1;
	};
}

