#pragma once

#include "Mathematics/vec2.h"

namespace chess
{
	namespace orient
	{
		static const uint Width			= 1280;
		static const uint Height		= 720;
		static const float squareSide	= 80.0f;
		static const float offsetX		= 10.0f;
		static const float offsetY		= 10.0f;
		static const float pieceScale	= squareSide / 320.0f;


		inline static math::vec2<> CoordsToRealCoords(uint x, uint y) {
			return math::vec2<>(offsetX + squareSide * x + squareSide / 2.0f, offsetY + squareSide * (7 - y) + squareSide / 2.0f);
		}

		inline static math::vec2<> CoordsToRealCoords(math::vec2<int> coords) {
			return CoordsToRealCoords(coords.x, coords.y);
		}

		inline static int PosFromCoords(int x, int y) {
			return 8 * y + x;
		}

		inline static int PosFromCoords(math::vec2<int> coords) {
			return 8 * coords.y + coords.x;
		}

		inline static math::vec2<int> CoordsFromPos(int pos) {
			return { pos % 8, pos / 8};
		}

		inline static int PosFromRealCoords(double x, double y) {
			return PosFromCoords(int((x - offsetX) / squareSide)  , 7 - int((y - offsetY) / squareSide));
		}

		inline static int PosFromRealCoords(math::vec2<double> coords) {
			return PosFromRealCoords(coords.x, coords.y);
		}
	}
}