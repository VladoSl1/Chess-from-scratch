#include "ChessManager.h"

#include <iostream>

#include "fenutil.h"
using namespace chess::fenutil;

int main()
{
	chess::ChessManager chess(1280, 720);
	
	while (!chess.WindowClose())
	{
		chess.Update();
	}

	return 0;
}

