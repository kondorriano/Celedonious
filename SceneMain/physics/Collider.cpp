#include "Collider.hpp"
#include "PhysicsEngine.hpp"
#include "PhysicsBody.hpp"

Collider::Collider() : pBody(nullptr) {
	pBody = PhysicsEngine::createBody();
	pBody->SetUserData(nullptr);
	setActive(true);
	setBullet(false);
	setAngularDamping(0.0f);
	setLinearDamping(0.0f);
	setGravityScale(1.0f);
	setRotation(0.0f);
	setPosition(vec2f(0.0f,0.0f));
	setDType(Dynamic);
}

Collider::~Collider() {
	PhysicsEngine::deleteBody(pBody);
	pBody = nullptr;
}

void Collider::init(PhysicsBody* pb) {
	VBE_ASSERT(pBody->GetUserData() == nullptr, "Trying to assign a collider to a physics object, but this collider was already assigned to some other object. Create a new collider instead.");
	pBody->SetUserData(pb);
}

void Collider::applyForce(const vec2f& f, const vec2f& p) {
	pBody->ApplyForce(Utils::GLMv2ToB2Dv2(f), Utils::GLMv2ToB2Dv2(p), true);
}

void Collider::applyForceToCenterOfMass(const vec2f& f) {
	pBody->ApplyForceToCenter(Utils::GLMv2ToB2Dv2(f), true);
}

void Collider::applyTorque(float torque) {
	pBody->ApplyTorque(torque, true);
}

void Collider::applyLinearImpulse(const vec2f& i, const vec2f& p) {
	pBody->ApplyLinearImpulse(Utils::GLMv2ToB2Dv2(i), Utils::GLMv2ToB2Dv2(p), true);
}

void Collider::applyAngularImpulse(float impulse) {
	pBody->ApplyAngularImpulse(impulse, true);
}

vec2f Collider::getCenterOfMass() const {
	return Utils::B2Dv2ToGLMv2(pBody->GetWorldCenter());
}

vec2f Collider::getPosition() const {
	return Utils::B2Dv2ToGLMv2(pBody->GetPosition());
}

void Collider::setPosition(vec2f pos) {
	pBody->SetTransform(Utils::GLMv2ToB2Dv2(pos), pBody->GetAngle());
}

float Collider::getRotation() const {
	return pBody->GetAngle();
}

void Collider::setRotation(float angle) {
	pBody->SetTransform(pBody->GetPosition(), angle);
}

void Collider::setTransform(const vec2f& pos, float angle) {
	pBody->SetTransform(Utils::GLMv2ToB2Dv2(pos), angle);
}

vec2f Collider::getLinearVelocity() const {
	return Utils::B2Dv2ToGLMv2(pBody->GetLinearVelocity());
}

void Collider::setLinearVelocity(const vec2f& lv) {
	pBody->SetLinearVelocity(Utils::GLMv2ToB2Dv2(lv));
}

float Collider::getAngularVelocity() const {
	return pBody->GetAngularVelocity();
}

void Collider::setAngularVelocity(float v) {
	pBody->SetAngularVelocity(v);
}

float Collider::getLinearDamping() const {
	return pBody->GetLinearDamping();
}

void Collider::setLinearDamping(float damping) {
	pBody->SetLinearDamping(damping);
}

float Collider::getAngularDamping() const {
	return pBody->GetAngularDamping();
}

void Collider::setAngularDamping(float damping) {
	pBody->SetAngularDamping(damping);
}

float Collider::getGravityScale() const {
	return pBody->GetGravityScale();
}

void Collider::setGravityScale(float scale) {
	pBody->SetGravityScale(scale);
}

Collider::DType Collider::getDType() const {
	return B2TypeToDType(pBody->GetType());
}

void Collider::setDType(Collider::DType t) {
	pBody->SetType(DTypeToB2Type(t));
}

bool Collider::isBullet() const {
	return pBody->IsBullet();
}

void Collider::setBullet(bool b) {
	pBody->SetBullet(b);
}

bool Collider::isFixedRotation() const {
	return pBody->IsFixedRotation();
}

void Collider::setFixedRotation(bool b) {
	pBody->SetFixedRotation(b);
}

bool Collider::isActive() const {
	return pBody->IsActive();
}

void Collider::setActive(bool b) {
	pBody->SetActive(b);
}

float Collider::getInertia() const {
	return pBody->GetInertia();
}

float Collider::getMass() const {
	return pBody->GetMass();
}

Collider::DType Collider::B2TypeToDType(b2BodyType t) const {
	switch(t) {
		case b2_dynamicBody: return Dynamic;
		case b2_staticBody: return Static;
		case b2_kinematicBody: return Kinematic;
		default: VBE_ASSERT(false, "Unknown b2BodyType");
	}
}

b2BodyType Collider::DTypeToB2Type(DType t) const {
	switch(t) {
		case Dynamic: return b2_dynamicBody;
		case Static: return b2_staticBody;
		case Kinematic: return b2_kinematicBody;
		default: VBE_ASSERT(false, "Unknown DType");
	}
}
