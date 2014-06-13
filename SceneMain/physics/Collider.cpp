#include "Collider.hpp"
#include "PhysicsEngine.hpp"
#include "PhysicsBody.hpp"

Collider::Collider() : pBody(nullptr), fixture(nullptr), shape(nullptr), node(nullptr), friction(0.1f), restitution(0.25f), density(1.0f), sensor(false) {
	pBody = PhysicsEngine::createBody();
	pBody->SetUserData(this);
	setActive(true);
	setBullet(false);
	setAngularDamping(0.0f);
	setLinearDamping(0.0f);
	setGravityScale(1.0f);
	setRotation(0.0f);
	setPosition(vec2f(0.0f,0.0f));
	setDType(Dynamic);
	setAngularVelocity(0.0f);
	setFixedRotation(false);
	setLinearVelocity(vec2f(0.0f));
}

Collider::~Collider() {
	PhysicsEngine::deleteBody(pBody);
	pBody = nullptr;
	if(shape != nullptr) delete shape;
}

PhysicsBody* Collider::getBody() {
	return node;
}

void Collider::init(PhysicsBody* pb) {
	VBE_ASSERT(node == nullptr, "Trying to assign a collider to a physics object, but this collider was already assigned to some other object. Create a new collider instead.");
	node = pb;
}

void Collider::remake() {
	if(fixture != nullptr) pBody->DestroyFixture(fixture);
	b2FixtureDef d;
	d.shape = shape;
	d.density = density;
	d.friction = friction;
	d.isSensor = sensor;
	d.restitution = restitution;
	d.userData = this;
	fixture = pBody->CreateFixture(&d);
	pBody->ResetMassData();
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

bool Collider::testPoint(vec2f point) {
	if(fixture == nullptr) return false;
	return fixture->TestPoint(Utils::GLMv2ToB2Dv2(point));
}

AABB Collider::getAABB() const {
	if(fixture == nullptr) return AABB();
	b2AABB aabb;
	b2Transform trans = fixture->GetBody()->GetTransform();
	shape->ComputeAABB(&aabb, trans, 0);
	return AABB(vec3f(aabb.lowerBound.x, aabb.lowerBound.y, 0), vec3f(aabb.upperBound.x, aabb.upperBound.y, 0));
}

void Collider::setDensity(float density) {
	this->density = density;
	if(fixture != nullptr) fixture->SetDensity(density);
	pBody->ResetMassData();
}

void Collider::setFriction(float friction) {
	this->friction = friction;
	if(fixture != nullptr) fixture->SetFriction(friction);
	pBody->ResetMassData();
}

void Collider::setRestitution(float restitution) {
	this->restitution = restitution;
	if(fixture != nullptr) fixture->SetRestitution(restitution);
	pBody->ResetMassData();
}

void Collider::setSensor(bool sensor) {
	this->sensor = sensor;
	if(fixture != nullptr) fixture->SetSensor(sensor);
	pBody->ResetMassData();
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
