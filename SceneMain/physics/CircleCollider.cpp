#include "CircleCollider.hpp"
#include "Engine.hpp"

namespace Physics {

	CircleCollider::CircleCollider() {
		b2CircleShape newshape;
		newshape.m_radius = 1.0f;
		type = Circle;
		remake(&newshape);
	}

	CircleCollider::~CircleCollider(){
	}

	float CircleCollider::getRadius() {
		return ((b2CircleShape*)shape)->m_radius;
	}

	void CircleCollider::setRadius(float r) {
		((b2CircleShape*)shape)->m_radius = r;
	}

	int CircleCollider::getVertexCount() const {
		return ((b2CircleShape*)shape)->GetVertexCount();
	}

	vec2f CircleCollider::getVertex(int index) const {
		VBE_ASSERT(index < getVertexCount(), "Circle Vertex: index out of bounds");
		return Utils::B2Dv2ToGLMv2(((b2CircleShape*)shape)->GetVertex(index));
	}

}
