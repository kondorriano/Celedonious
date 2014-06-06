#ifndef POLYGONCOLLIDER_HPP
#define POLYGONCOLLIDER_HPP
#include "Collider.hpp"

class PolygonCollider : public Collider {
	public:
		PolygonCollider();
		~PolygonCollider();

		void setAsBox(float hx, float hy);
		void setAsBox(float hx, float hy, const vec2f& center, float angle);
		int getVertexCount() const;
		vec2f getLocalVertex(int index) const;
		vec2f getWorldVertex(int index) const;
};

#endif // POLYGONCOLLIDER_HPP
