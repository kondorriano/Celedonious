#include "PolygonCollider.hpp"
#include "PhysicsEngine.hpp"

PolygonCollider::PolygonCollider() : fixture(nullptr) {
	setAsBox(1.0,1.0);
	setSensor(false);
	setDensity(1.0f);
	setFriction(0.1f);
	setRestitution(1.0f);
}

PolygonCollider::~PolygonCollider(){
}

void PolygonCollider::setDensity(float density) {
	this->density = density;
	fixture->SetDensity(density);
	pBody->ResetMassData();
}

void PolygonCollider::setFriction(float friction) {
	this->friction = friction;
	fixture->SetFriction(friction);
	pBody->ResetMassData();
}

void PolygonCollider::setRestitution(float restitution) {
	this->restitution = restitution;
	fixture->SetRestitution(restitution);
	pBody->ResetMassData();
}

void PolygonCollider::setSensor(bool sensor) {
	this->sensor = sensor;
	fixture->SetSensor(sensor);
	pBody->ResetMassData();
}

void PolygonCollider::remake() {
	if(fixture != nullptr) pBody->DestroyFixture(fixture);
	b2FixtureDef d;
	d.shape = &shape;
	fixture = pBody->CreateFixture(&d);
	pBody->ResetMassData();
}

void PolygonCollider::setAsBox(float hx, float hy){
	shape.SetAsBox(hx, hy);
	remake();
}

void PolygonCollider::setAsBox(float hx, float hy, const vec2f& center, float angle){
	shape.SetAsBox(hx, hy, Utils::GLMv2ToB2Dv2(center), angle);
	remake();
}

int PolygonCollider::getVertexCount() const {
	return shape.GetVertexCount();
}

vec2f PolygonCollider::getVertex(int index) const {
	VBE_ASSERT(index < getVertexCount(), "Polygon Vertex: index out of bounds");
	return Utils::B2Dv2ToGLMv2(shape.GetVertex(index));
}
