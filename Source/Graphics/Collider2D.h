#pragma once

#include "Core/Collider.h"
#include "LineRenderer.h"

namespace graphics
{
	class PolygonCollider : public core::Collider
	{
	public:
		PolygonCollider(bool connectLines) : connectLines(connectLines) { }
		PolygonCollider(size_t size, bool connectLines) : Collider(size), connectLines(connectLines) { }
		PolygonCollider(std::vector<math::vec2<>> _points, bool connectLines)
			: Collider(_points.size()), connectLines(connectLines)
		{
			addPoints(_points);
		}

		bool collision(const PolygonCollider& collider) const
		{
			int step_one = connectLines ? 1 : 2;
			int step_two = collider.connectLines ? 1 : 2;
			
			for (size_t i = 0; i < points.size() - 1; i += step_one)
				for (size_t j = 0; j < collider.points.size() - 1; j += step_two)
					if (lineIntersection(points[i], points[i + 1], collider.points[j], collider.points[j + 1]))
						return true;
			return false;
		}

		void Draw(LineRenderer* lineRenderer, math::vec3<> color = math::vec3(0.0f, 1.0f, 0.0f))
		{
			int step = connectLines ? 1 : 2;
			for (size_t i = 0; i < points.size() - 1; i += step)
				lineRenderer->DrawLine(points[i], points[i + 1], color);
		}

		inline PolygonCollider& operator=(const PolygonCollider& collider) {
			points = collider.points;
 			return *this;
		}

	private:
		const bool connectLines;
	};

	class BoxCollider : public PolygonCollider
	{
	public:
		BoxCollider() : PolygonCollider(5, true) { }
		BoxCollider(math::vec2<> _start, math::vec2<> _end) : PolygonCollider(5, true) {
			addPoints({ _start, { _end.x, _start.y }, _end, { _start.x, _end.y }, _start });
		}
	};
}