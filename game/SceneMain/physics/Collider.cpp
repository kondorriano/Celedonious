#include "Collider.hpp"
#include "Engine.hpp"
#include "Body.hpp"
#include "Joint.hpp"

namespace Physics {

	Collider::Collider() : pBody(nullptr), fixture(nullptr), shape(nullptr), node(nullptr), friction(0.1f), restitution(0.25f), density(1.0f), sensor(false) {
		pBody = Engine::createBody();
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
		Engine::deleteBody(pBody);
	}

	Body* Collider::getBody() {
		return node;
	}

	const Body* Collider::getBody() const {
		return node;
	}

	Joint* Collider::getJoint(unsigned int index) {
		VBE_ASSERT((int) index < getJointCount(), "Invalid joint index");
		return joints[index];
	}

	const Joint* Collider::getJoint(unsigned int index) const {
		VBE_ASSERT((int) index < getJointCount(), "Invalid joint index");
		return joints[index];
	}

	void Collider::deleteJoint(unsigned int index) {
		VBE_ASSERT((int) index < getJointCount(), "Invalid joint index");
		Physics::Collider* a = joints[index]->cA;
		Physics::Collider* b = joints[index]->cB;
		Joint* joint = joints[index];
		delete joint;
		a->joints.erase(a->joints.begin()+a->getJointIndex(joint));
		b->joints.erase(b->joints.begin()+b->getJointIndex(joint));
	}

	int Collider::getJointCount() const {
		return joints.size();
	}

	int Collider::getJointIndex(const Joint* joint) const {
		for(unsigned int i = 0; i < joints.size(); ++i) if(joints[i] == joint) return i;
		return -1;
	}

	void Collider::init(Body* pb) {
		VBE_ASSERT(node == nullptr, "Trying to assign a collider to a physics object, but this collider was already assigned to some other object. Create a new collider instead.");
		node = pb;
	}

	void Collider::remake(b2Shape* newShape) {
		if(fixture != nullptr) pBody->DestroyFixture(fixture);
		b2FixtureDef d;
		d.shape = newShape;
		d.density = density;
		d.friction = friction;
		d.isSensor = sensor;
		d.restitution = restitution;
		d.userData = this;
		fixture = pBody->CreateFixture(&d);
		pBody->ResetMassData();
		shape = fixture->GetShape();
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
		return fixture->TestPoint(Utils::GLMv2ToB2Dv2(point));
	}

	std::pair<float, vec2f> Collider::computeDistance(const vec2f& p) const {
		std::pair<float, vec2f> ret;
		fixture->ComputeDistance(Utils::GLMv2ToB2Dv2(p), &ret.first, (b2Vec2*)&ret.second, 0);
		return ret;
	}

	AABB Collider::getAABB() const {
		b2AABB aabb = fixture->GetAABB(0);
		return AABB(vec3f(aabb.lowerBound.x, aabb.lowerBound.y, 0), vec3f(aabb.upperBound.x, aabb.upperBound.y, 0));
	}

	vec2f Collider::getWorldPoint(const vec2f& localPoint) const {
		return Utils::B2Dv2ToGLMv2(pBody->GetWorldPoint(Utils::GLMv2ToB2Dv2(localPoint)));
	}

	vec2f Collider::getWorldVector(const vec2f& localVector) const {
		return Utils::B2Dv2ToGLMv2(pBody->GetWorldVector(Utils::GLMv2ToB2Dv2(localVector)));
	}

	vec2f Collider::getLocalPoint(const vec2f& worldPoint) const {
		return Utils::B2Dv2ToGLMv2(pBody->GetLocalPoint(Utils::GLMv2ToB2Dv2(worldPoint)));
	}

	vec2f Collider::getLocalVector(const vec2f& worldVector) const {
		return Utils::B2Dv2ToGLMv2(pBody->GetLocalVector(Utils::GLMv2ToB2Dv2(worldVector)));
	}

	vec2f Collider::getLinearVelocityFromWorldPoint(const vec2f& worldPoint) const {
		return Utils::B2Dv2ToGLMv2(pBody->GetLinearVelocityFromWorldPoint(Utils::GLMv2ToB2Dv2(worldPoint)));
	}

	vec2f Collider::getLinearVelocityFromLocalPoint(const vec2f& localPoint) const {
		return Utils::B2Dv2ToGLMv2(pBody->GetLinearVelocityFromLocalPoint(Utils::GLMv2ToB2Dv2(localPoint)));
	}

	void Collider::setDensity(float density) {
		this->density = density;
		fixture->SetDensity(density);
		pBody->ResetMassData();
	}

	void Collider::setFriction(float friction) {
		this->friction = friction;
		fixture->SetFriction(friction);
		pBody->ResetMassData();
	}

	void Collider::setRestitution(float restitution) {
		this->restitution = restitution;
		fixture->SetRestitution(restitution);
		pBody->ResetMassData();
	}

	void Collider::setSensor(bool sensor) {
		this->sensor = sensor;
		fixture->SetSensor(sensor);
		pBody->ResetMassData();
	}

	float Collider::getRadius() const {
		return shape->m_radius;
	}

	void Collider::setRadius(float r) {
		shape->m_radius = r;
	}

	vec2f Collider::getWorldCenterOfMass() const {
		return Utils::B2Dv2ToGLMv2(pBody->GetWorldCenter());
	}

	vec2f Collider::GetLocalCenterOfMass() const {
		return Utils::B2Dv2ToGLMv2(pBody->GetLocalCenter());
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
		return (Collider::DType) pBody->GetType();
	}

	void Collider::setDType(Collider::DType t) {
		pBody->SetType((b2BodyType)t);
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

	bool Collider::isSleepingAllowed() const {
		return pBody->IsSleepingAllowed();
	}

	void Collider::setSleepingAllowed(bool flag) {
		pBody->SetSleepingAllowed(flag);
	}

	bool Collider::isAwake() const {
		return pBody->IsAwake();
	}

	void Collider::setAwake(bool b) {
		pBody->SetAwake(b);
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

	float Collider::getAngle() const {
		return pBody->GetAngle();
	}

}
