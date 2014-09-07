#include "PolygonCollider.hpp"
#include "Engine.hpp"

namespace Physics {

	PolygonCollider::PolygonCollider() {
		b2PolygonShape s;
		type = Polygon;
		s.SetAsBox(1.0,1.0);
		remake(&s);
	}

	PolygonCollider::~PolygonCollider(){
	}

	void PolygonCollider::set(const vec2f* points, int count) {
		((b2PolygonShape*)shape)->Set((b2Vec2*) points, count);
	}

	void PolygonCollider::setAsBox(float hx, float hy){
		((b2PolygonShape*)shape)->SetAsBox(hx, hy);
	}

	void PolygonCollider::setAsBox(float hx, float hy, const vec2f& center, float angle){
		((b2PolygonShape*)shape)->SetAsBox(hx, hy, Utils::GLMv2ToB2Dv2(center), angle);
	}

	int PolygonCollider::getVertexCount() const {
		return ((b2PolygonShape*)shape)->GetVertexCount();
	}

	vec2f PolygonCollider::getLocalVertex(int index) const {
		VBE_ASSERT(index < getVertexCount(), "Polygon Vertex: index out of bounds");
		return Utils::B2Dv2ToGLMv2(((b2PolygonShape*)shape)->GetVertex(index));
	}

	vec2f PolygonCollider::getWorldVertex(int index) const {
		VBE_ASSERT(index < getVertexCount(), "Polygon Vertex: index out of bounds");
		return Utils::B2Dv2ToGLMv2(pBody->GetWorldPoint(((b2PolygonShape*)shape)->GetVertex(index)));
	}

}
