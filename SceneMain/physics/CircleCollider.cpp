#include "CircleCollider.hpp"
#include "PhysicsEngine.hpp"

CircleCollider::CircleCollider() {
	shape = new b2CircleShape();
	type = Circle;
	setRadius(1.0f);
}

CircleCollider::~CircleCollider(){
}

float CircleCollider::getRadius() {
	return ((b2CircleShape*)shape)->m_radius;
}

void CircleCollider::setRadius(float r) {
	((b2CircleShape*)shape)->m_radius = r;
	remake();
}

int CircleCollider::getVertexCount() const {
	return ((b2CircleShape*)shape)->GetVertexCount();
}

vec2f CircleCollider::getVertex(int index) const {
	VBE_ASSERT(index < getVertexCount(), "Circle Vertex: index out of bounds");
	return Utils::B2Dv2ToGLMv2(((b2CircleShape*)shape)->GetVertex(index));
}
