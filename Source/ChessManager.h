#pragma once

#include "Graphics/Window.h"
#include "Graphics/Keyboard.h"

#include "Board.h"

namespace chess
{
	class ChessManager
	{
	public:
		ChessManager(unsigned int width, unsigned int height);
		~ChessManager();

		void Update();

		inline bool WindowClose() const {
			return window->WindowShouldClose();
		}

	private:
		graphics::Window* window;
		graphics::Mouse* mouse;
		
		Board* board;

		pallet::SquarePallet* boardColorPallet;
		pallet::PiecePallet* boardPiecePallet;

		bool draggingPiece = false;

		void ProcessMouseEvents();
	};
}

