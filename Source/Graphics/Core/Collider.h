#pragma once

#include "Mathematics/vec2.h"

#include <vector>

namespace graphics
{
	namespace core
	{
		class Collider
		{
		public:
			Collider() = default;
			Collider(size_t size) { points.reserve(size); }

			inline void addPoint(math::vec2<> point) {
				points.push_back(point);
			}

			inline void addPoints(std::vector<math::vec2<>> _points) {
				points.insert(points.end(), _points.begin(), _points.end());
			}

			/* Collision of two colliders defined with continuous lines */
			bool collision(const Collider& collider) const
			{
				for (size_t i = 0; i < points.size() - 1; ++i)
					for (size_t j = 0; j < collider.points.size() - 1; ++j)
						if (lineIntersection(points[i], points[i + 1], collider.points[j], collider.points[j + 1]))
							return true;
			}

			inline size_t size() const { return points.size(); }
			
			inline const math::vec2<>& operator[](size_t i) const { return points[i]; }
			
			inline math::vec2<>& operator[](size_t i) {
				return const_cast<math::vec2<>&>(static_cast<const Collider&>(*this)[i]);
			}
			
			inline Collider& operator=(const Collider& collider) {
				points = collider.points;
				return *this;
			}

		protected:
			/* Checks if Line1(s1, e1) intersects Line2(s2, e2) in 2D space */
			bool lineIntersection(const math::vec2<>& s1, const math::vec2<>& e1, const math::vec2<>& s2, const math::vec2<>& e2) const
			{
				float det = ((s1.x - e1.x) * (s2.y - e2.y) - (s1.y - e1.y) * (s2.x - e2.x));
				
				if (det == 0)			// both lines are vertical
				{
					float x1 = math::min(s1.x, e1.x);
					float x2 = math::max(s1.x, e1.x);
					float x3 = math::min(s2.x, e2.x);
					float x4 = math::max(s2.x, e2.x);

					if ((s1.y - s2.y == 0) && ((x4 >= x1) || (x2 >= x3)))
						return true;
				}

				float _t  = ((s1.x - s2.x) * (s2.y - e2.y) - (s1.y - s2.y) * (s2.x - e2.x)) / det;
				float _u  = ((e1.x - s1.x) * (s1.y - s2.y) - (e1.y - s1.y) * (s1.x - s2.x)) / det;
				return (0.0f <= _t) && (_t <= 1.0f) && (0.0f <= _u) && (_u <= 1.0f);
			}

			std::vector<math::vec2<>> points;
		};
	}
}