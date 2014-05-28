#ifndef POLYGONCOLLIDER_HPP
#define POLYGONCOLLIDER_HPP
#include "Collider.hpp"

class PolygonCollider : public Collider {
	public:
		PolygonCollider();
		~PolygonCollider();

		void setDensity(float density);
		void setFriction(float friction);
		void setRestitution(float restitution);
		void setSensor(bool sensor);

		void setAsBox(float hx, float hy);
		void setAsBox(float hx, float hy, const vec2f& center, float angle);
		int getVertexCount() const;
		vec2f getVertex(int index) const;
	private:
		void remake();

		b2PolygonShape shape;
		b2Fixture* fixture;
};

#endif // POLYGONCOLLIDER_HPP
