#ifndef POLYGONCOLLIDER_HPP
#define POLYGONCOLLIDER_HPP
#include "Collider.hpp"

namespace Physics {

	class PolygonCollider : public Collider {
		public:
			PolygonCollider();
			~PolygonCollider();

			void set(const vec2f* points, int count);
			void setAsBox(float hx, float hy);
			void setAsBox(float hx, float hy, const vec2f& center, float angle);
			int getVertexCount() const;
			vec2f getLocalVertex(int index) const;
			vec2f getWorldVertex(int index) const;
	};

}

#endif // POLYGONCOLLIDER_HPP
