#include "Board.h"

#include "Orientation.h"
#include "Graphics/Core/BasicModels.h"

#include "MoveGenerator.h"

#include <algorithm>

using namespace math;
using namespace graphics;


/*
- 
- 
- 
*/


namespace chess
{
	Board::Board(Side orientation, pallet::SquarePallet* boardColorPallet, pallet::PiecePallet* piecePallet)
		: orient(orientation), boardColors(boardColorPallet), pieceTextures(piecePallet), checkMate(false)
	{
		spriteRenderer = new SpriteRenderer(orient::Width, orient::Height, false);

		InitGraphics(boardColorPallet);
	}

	Board::~Board()
	{
		delete posInfo;
		delete spriteRenderer;
	}

	void Board::LoadPosition(const std::string& fen_position)
	{
		posInfo = fenutil::LoadPosition(fen_position);

		white.pieces.clear();
		black.pieces.clear();

		uchar piece;
		Texture2D temp;
		for (char i = 0; i < 64; ++i)
		{
			piece = posInfo->squares[i];
			if (piece)
			{
				
				PieceSet* pieceSet = GetPieceSet(Pieces::IsWhite(piece));
				pieceSet->pieces.insert(i);
				if (Pieces::IsType(piece, Pieces::King))
					pieceSet->kingPos = i;
				 
				temp = *pieceTextures->GetTexture(piece);
				g_pieces[i].size = vec2<>((float)temp.width, (float)temp.height) * orient::pieceScale;
				g_pieces[i].texture = temp;
			}
		}
	}

	void Board::Draw()
	{	
		for (uchar i = 0; i < 64; ++i)
			g_squares[i].Draw(spriteRenderer);

		for (char piece : white.pieces)
			g_pieces[piece].Draw(spriteRenderer);
		
		for (char piece : black.pieces)
			g_pieces[piece].Draw(spriteRenderer);

		if (draggedPiece != -1)
			g_pieces[draggedPiece].Draw(spriteRenderer);
	}

	/*
	file = column (A-H)
	rank = row (1-8)
	*/

	void Board::InitGraphics(pallet::SquarePallet* colorPallet)
	{
		baseSquareTexture = Texture2D("Resources/Textures/white_square.png");

		vec3<> whiteColor = boardColors->whiteSquareColor;
		vec3<> blackColor = boardColors->blackSquareColor;

		vec2 pos;
		vec3 color;
		uchar file, rank;
		for (char i = 0; i < 64; ++i)
		{
			rank = i % 8;
			file = i / 8;

			pos = orient::CoordsToRealCoords(rank, file);
			color = (file + rank) % 2 ? whiteColor : blackColor;

			g_squares[i]	= GameObject(pos, vec2(orient::squareSide), baseSquareTexture, color, true);
			g_pieces[i]		= GameObject(pos, vec2(0.0f, 0.0f), baseSquareTexture, vec3(1.0f), true);
		}
	}

	void Board::StartPieceDrag(int pos, const math::vec2<>& coords)
	{
		if (Pieces::IsColor(posInfo->squares[pos], Pieces::White) == posInfo->whiteOnMove &&
			!vector::isIn(dict::keys(movegen::pinnedPieces), pos))
		{
			dragShift = coords - g_pieces[pos].position;
			draggedPiece = pos;

			std::set<int> intersect; // = std::set<int>();
			possibleMoves = movegen::GenerateMoves(pos, posInfo);
			
			//if ()

			if (kingAttacker >= 0 && !Pieces::IsType(posInfo->squares[pos], Pieces::King))
			{
				// make intersection counterMoves-possibleMoves to obtain set of only valid moves
				std::set_intersection(possibleMoves.begin(), possibleMoves.end(),
					counterMoves.begin(), counterMoves.end(), std::inserter(intersect, intersect.begin()));

				for (auto i : counterMoves) {
					std::cout << i << " ";
				}
				std::cout << "\n";

				possibleMoves = intersect;
			}

			else
			{
				if (Pieces::IsType(posInfo->squares[pos], Pieces::King))
				{
					std::vector<int> attackedSq = dict::keys(attackedSquares);
					std::sort(attackedSq.begin(), attackedSq.end());

					// king cannot go on attacked squares
					std::set_difference(possibleMoves.begin(), possibleMoves.end(),
						attackedSq.begin(), attackedSq.end(), std::inserter(intersect, intersect.begin()));

					// king cannot castle when in check
					if (kingAttacker != -1 &&
						posInfo->castling & 0b1100 && Pieces::IsWhite(posInfo->squares[pos]) ||
						posInfo->castling & 0b0011 && !Pieces::IsWhite(posInfo->squares[pos]))
					{
						intersect.erase(pos + 2);
						intersect.erase(pos - 2);
					}

					possibleMoves = intersect;
				}

				else if (kingAttacker == -2)
				{
					possibleMoves.clear();
				}
			}

			// mark attacked squares
			for (int i : possibleMoves)
			{
				bool white = (i % 8 + int(i / 8)) % 2 == 1;
				g_squares[i].color = white ? boardColors->whiteSquareAttacked : boardColors->blackSquareAttacked;
			}
		}
	}

	void Board::EndPieceDrag(int pos)
	{
		if (possibleMoves.find(pos) != possibleMoves.end()) {
			//bool w = Pieces::IsWhite(posInfo->squares[pos]);
			
			MovePiece(draggedPiece, pos);

			attackedSquares = GetAttackedSquares(posInfo->whiteOnMove);

			posInfo->whiteOnMove ^= 1;

			UpdateKingState(posInfo->whiteOnMove);
			checkMate = IsCheckMate();
		}
		else
			g_pieces[draggedPiece].position = orient::CoordsToRealCoords(orient::CoordsFromPos(draggedPiece));
		
		for (int i : possibleMoves)
		{
			bool white = (i % 8 + int(i / 8)) % 2 == 1;
			g_squares[i].color = white ? boardColors->whiteSquareColor : boardColors->blackSquareColor;
		}

		draggedPiece = -1;
		possibleMoves = std::set<int>();
		
	} 

	void Board::MovePiece(int start, int end)
	{
		if (Pieces::IsType(posInfo->squares[start], Pieces::Pawn)) {
			if (!PawnEnPassant(start, end))
				posInfo->enPassant = 0;
		}
		else
		{
			posInfo->enPassant = 0;
			if (Pieces::IsType(posInfo->squares[start], Pieces::King)) {
				RookCastling(start, end);
				KingCastling(start, end);
			}
		}

		if ((posInfo->whiteOnMove && Pieces::IsColor(posInfo->squares[end], Pieces::Black)) ||
			(!posInfo->whiteOnMove && Pieces::IsColor(posInfo->squares[end], Pieces::White)))
		{
			GetPieceSet(!posInfo->whiteOnMove)->pieces.erase(end);
		}

		GetPieceSet(posInfo->whiteOnMove)->pieces.erase(start);
		GetPieceSet(posInfo->whiteOnMove)->pieces.insert(end);
		
		posInfo->squares[end]		= posInfo->squares[start];
		posInfo->squares[start]		= 0;
		g_pieces[end]				= g_pieces[start];
		g_pieces[end].position		= g_squares[end].position;
	}


	void Board::UpdateKingState(bool color_w)
	{
		kingAttacker = -1;
		counterMoves.clear();
		char kingPos = GetPieceSet(color_w)->kingPos;

		/* if king in check: -2 if double-check; else pos of attacker */
		if (attackedSquares.find(kingPos) != attackedSquares.end())
			kingAttacker = attackedSquares[kingPos].size() > 1 ? -2 : *next(attackedSquares[kingPos].begin(), 0);

		/* single attacker */
		if (kingAttacker >= 0)
		{
			counterMoves.insert(kingAttacker);
			if (Pieces::IsSlidingPiece(posInfo->squares[kingAttacker]))
			{
				int stepX = kingPos % 8 - kingAttacker % 8;
				int stepY = int(kingPos / 8) - int(kingAttacker / 8);
				int n_steps = max(stepX, stepY);

				stepX = stepX < 0 ? -1 : (stepX > 0 ? 1 : 0);
				stepY = stepY < 0 ? -1 : (stepY > 0 ? 1 : 0);
				for (int i = 1; i < n_steps; ++i) {
					counterMoves.insert(kingAttacker + stepX * i + 8 * stepY * i);
				}
			}
		}
	}

	Board::AttackMap Board::GetAttackedSquares(bool color_w) const
	{
		AttackMap attackedSquareMap;
		movegen::pinnedPieces.clear();

		for (char piece : GetPieceSet(color_w)->pieces)
			for (int square : movegen::GenerateMoves(piece, posInfo, true))
				attackedSquareMap[square].insert(piece);

		return attackedSquareMap;
	}

	/* returns true if enPassant option is created, false otherwise */
	bool Board::PawnEnPassant(int start, int end)
	{
		//bool w = Pieces::IsColor(posInfo->squares[start], Pieces::White);
		if (abs(start - end) == 16)
		{
			posInfo->enPassant = start - (start - end) / 2;
			return true;
		}
		
		else if (end == posInfo->enPassant)
		{
			posInfo->squares[int(end + 8 * pow(-1, posInfo->whiteOnMove))] = 0;
			GetPieceSet(!posInfo->whiteOnMove)->pieces.erase(char(end + 8 * pow(-1, posInfo->whiteOnMove)));
		}

		return false;
	}

	void Board::RookCastling(int start, int end)
	{
		if (posInfo->castling)
		{
			if (posInfo->castling & 0b0001 && end == 58)
				MovePiece(56, 59);
			else if (posInfo->castling & 0b0010 && end == 62)
				MovePiece(63, 61);
			else if (posInfo->castling & 0b0100 && end == 2)
				MovePiece(0, 3);
			else if (posInfo->castling & 0b1000 && end == 6)
				MovePiece(7, 5);

			// condition for castling is unchanged rook state
			if (start == 0 || end == 0)
				posInfo->castling &= 0b1011;
			else if (start == 7 || end == 7)
				posInfo->castling &= 0b0111;
			else if (start == 56 || end == 56)
				posInfo->castling &= 0b1110;
			else if (start == 63 || end == 63)
				posInfo->castling &= 0b1101;
		}
	}

	void Board::KingCastling(int start, int end)
	{
		(posInfo->whiteOnMove ? white : black).kingPos = end;			
		posInfo->castling &= 0b1100 >> (posInfo->whiteOnMove * 2);		// condition for castling is unchanged king state
	}

	bool Board::IsCheckMate() const {
		for (int pos = 0; pos < 64; ++pos) {
			if (!movegen::GenerateMoves(pos, posInfo, false).empty())
				return false;
		}
		return true;
	}
}
