#include "ChessManager.h"

#include "fenutil.h"
#include "Orientation.h"

#include "Mathematics/vec4.h"

using namespace math;
using namespace graphics;


namespace chess
{
	ChessManager::ChessManager(unsigned int width, unsigned int height)
	{
		window = new Window(width, height, "Chess 0.1", false);
		mouse = new Mouse(window->GetWindow(), 1);
		mouse->BindKey(GLFW_MOUSE_BUTTON_1);

		boardColorPallet = new pallet::SquarePallet(
			vec3<>(255.0f, 211.0f, 146.0f),
			vec3<>(134.0f, 89.0f, 45.0f),
			vec3<>(255.0f, 148.0f, 148.0f),
			vec3<>(191.0f, 64.0f, 64.0f)
		);

		std::string pieceTexPaths[12];
		for (uchar i = 0; i < 12; ++i)
			pieceTexPaths[i] = "Resources/Textures/Pieces/Pieces_" + std::to_string(i) + ".png";

		boardPiecePallet = new pallet::PiecePallet(pieceTexPaths);

		board = new Board(Side::white, boardColorPallet, boardPiecePallet);
		board->LoadPosition(fenutil::StartingPosition);
	}

	ChessManager::~ChessManager()
	{
		delete mouse;
		delete board;
		delete boardColorPallet;
		delete boardPiecePallet;
		delete window;
	}


	void ChessManager::Update()
	{
		window->ClearScreen(vec4<>(0.5f));

		ProcessMouseEvents();

		board->Draw();
		window->Update();
		window->ShowFPS();
	}

	void ChessManager::ProcessMouseEvents()
	{
		KeyData kd = mouse->GetButtonState(GLFW_MOUSE_BUTTON_1);
		int pos = orient::PosFromRealCoords(mouse->pos);

		if (pos >= 0 && pos < 64)
		{
			if ((uchar)kd.state & (uchar)ButtonStates::ImmediateChange)
			{
				if ((uchar)kd.state & (uchar)ButtonStates::Release)
				{
					board->EndPieceDrag(pos);
					draggingPiece = false;
				}

				else if (board->g_pieces[pos].IsIn(mouse->pos))
				{
					board->StartPieceDrag(pos, mouse->pos);
					draggingPiece = true;
				}
			}

			else if ((uchar)kd.state & (uchar)ButtonStates::Press)
				board->DragPiece(mouse->pos);
		}

		else
		{
			if (draggingPiece)
			{
				draggingPiece = false;
				board->EndPieceDrag(-1);
			}
		}
	}
}
