#ifndef CIRCLECOLLIDER_HPP
#define CIRCLECOLLIDER_HPP
#include "Collider.hpp"

class CircleCollider : public Collider {
	public:
		CircleCollider();
		~CircleCollider();

		void setDensity(float density);
		void setFriction(float friction);
		void setRestitution(float restitution);
		void setSensor(bool sensor);

		void setRadius(float r);

		int getVertexCount() const;
		vec2f getVertex(int index) const;
	private:
		void remake();

		b2CircleShape shape;
		b2Fixture* fixture;
};

#endif // CIRCLECOLLIDER_HPP
