#include "ChessManager.h"

#include <iostream>


#include "fenutil.h"
using namespace chess::fenutil;

int main()
{
	chess::ChessManager chess(chess::orient::Width, chess::orient::Height);
	
	while (!chess.WindowClose())
	{
		chess.Update();
	}

	return 0;
}

