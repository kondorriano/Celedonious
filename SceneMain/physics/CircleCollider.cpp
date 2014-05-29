#include "CircleCollider.hpp"
#include "PhysicsEngine.hpp"

CircleCollider::CircleCollider() : fixture(nullptr) {
	setRadius(1.0f);
}

CircleCollider::~CircleCollider(){
}

void CircleCollider::setDensity(float density) {
	this->density = density;
	if(fixture != nullptr) fixture->SetDensity(density);
	pBody->ResetMassData();
}

void CircleCollider::setFriction(float friction) {
	this->friction = friction;
	if(fixture != nullptr) fixture->SetFriction(friction);
	pBody->ResetMassData();
}

void CircleCollider::setRestitution(float restitution) {
	this->restitution = restitution;
	if(fixture != nullptr) fixture->SetRestitution(restitution);
	pBody->ResetMassData();
}

void CircleCollider::setSensor(bool sensor) {
	this->sensor = sensor;
	if(fixture != nullptr) fixture->SetSensor(sensor);
	pBody->ResetMassData();
}

void CircleCollider::setRadius(float r) {
	shape.m_radius = r;
	remake();
}

void CircleCollider::remake() {
	b2FixtureDef d;
	d.density = density;
	d.friction = friction;
	d.isSensor = sensor;
	d.restitution = restitution;
	d.shape = &shape;
	if(fixture != nullptr) pBody->DestroyFixture(fixture);
	fixture = pBody->CreateFixture(&d);
	pBody->ResetMassData();
}

int CircleCollider::getVertexCount() const {
	return shape.GetVertexCount();
}

vec2f CircleCollider::getVertex(int index) const {
	VBE_ASSERT(index < getVertexCount(), "Polygon Vertex: index out of bounds");
	return Utils::B2Dv2ToGLMv2(shape.GetVertex(index));
}
