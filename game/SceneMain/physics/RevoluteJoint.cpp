#include "RevoluteJoint.hpp"
#include "Collider.hpp"
#include "Engine.hpp"

namespace Physics {

	RevoluteJoint::RevoluteJoint(const RevoluteJointDef& def) : Joint(Joint::Revolute) {
		cA = def.colliderA;
		cB = def.colliderB;
		cA->joints.push_back(this);
		cB->joints.push_back(this);
		b2RevoluteJointDef d;
		d.bodyA = cA->pBody;
		d.bodyB = cB->pBody;
		d.collideConnected = def.collideConnected;
		d.enableLimit = def.enableLimit;
		d.enableMotor = def.enableMotor;
		d.lowerAngle = def.lowerAngle;
		d.upperAngle = def.upperAngle;
		d.maxMotorTorque = def.maxMotorTorque;
		d.motorSpeed = def.motorSpeed;
		d.referenceAngle = def.referenceAngle;
		d.userData = this;
		d.localAnchorA = Utils::GLMv2ToB2Dv2(def.localAnchorA);
		d.localAnchorB = Utils::GLMv2ToB2Dv2(def.localAnchorB);
		joint = Engine::createJoint(&d);
	}

	RevoluteJoint::~RevoluteJoint() {
	}

	vec2f RevoluteJoint::getAnchorA() const {
		return Utils::B2Dv2ToGLMv2(((b2RevoluteJoint*)joint)->GetAnchorA());
	}

	vec2f RevoluteJoint::getAnchorB() const {
		return Utils::B2Dv2ToGLMv2(((b2RevoluteJoint*)joint)->GetAnchorB());
	}

	vec2f RevoluteJoint::getLocalAnchorA() const {
		return Utils::B2Dv2ToGLMv2(((b2WeldJoint*)joint)->GetLocalAnchorA());
	}

	vec2f RevoluteJoint::getLocalAnchorB() const {
		return Utils::B2Dv2ToGLMv2(((b2WeldJoint*)joint)->GetLocalAnchorB());
	}

	vec2f RevoluteJoint::getReactionForce(float inverseDelta) const {
		return Utils::B2Dv2ToGLMv2(((b2RevoluteJoint*)joint)->GetReactionForce(inverseDelta));
	}

	float RevoluteJoint::getReactionTorque(float inverseDelta) const {
		return ((b2RevoluteJoint*)joint)->GetReactionTorque(inverseDelta);
	}

	float RevoluteJoint::getJointAngle() const {
		return ((b2RevoluteJoint*)joint)->GetJointAngle();
	}

	float RevoluteJoint::getJointSpeed() const {
		return ((b2RevoluteJoint*)joint)->GetJointSpeed();
	}

	float RevoluteJoint::getLowerLimit() const {
		return ((b2RevoluteJoint*)joint)->GetLowerLimit();
	}

	float RevoluteJoint::getUpperLimit() const {
		return ((b2RevoluteJoint*)joint)->GetUpperLimit();
	}

	float RevoluteJoint::getMaxMotorTorque() const {
		return ((b2RevoluteJoint*)joint)->GetMaxMotorTorque();
	}

	float RevoluteJoint::getMotorSpeed() const {
		return ((b2RevoluteJoint*)joint)->GetMotorSpeed();
	}

	float RevoluteJoint::getMotorTorque(float inverseDeltatime) const {
		return ((b2RevoluteJoint*)joint)->GetMotorTorque(inverseDeltatime);
	}

	float RevoluteJoint::getReferenceAngle() const {
		return ((b2RevoluteJoint*)joint)->GetReferenceAngle();
	}

	void RevoluteJoint::setLimits(float lower, float upper) const {
		((b2RevoluteJoint*)joint)->SetLimits(lower, upper);
	}

	void RevoluteJoint::setMaxMotorTorque(float torque) const {
		((b2RevoluteJoint*)joint)->SetMaxMotorTorque(torque);
	}

	void RevoluteJoint::setMotorSpeed(float speed) const {
		((b2RevoluteJoint*)joint)->SetMotorSpeed(speed);
	}

	void RevoluteJoint::enableLimit(bool flag) const {
		((b2RevoluteJoint*)joint)->EnableLimit(flag);
	}

	void RevoluteJoint::enableMotor(bool flag) const {
		((b2RevoluteJoint*)joint)->EnableMotor(flag);
	}

	bool RevoluteJoint::isLimitEnabled() const {
		return ((b2RevoluteJoint*)joint)->IsLimitEnabled();
	}

	bool RevoluteJoint::isMotorEnabled() const {
		return ((b2RevoluteJoint*)joint)->IsMotorEnabled();
	}

	void RevoluteJointDef::init(Collider* cA, Collider* cB, const vec2f& anchor) {
		colliderA = cA;
		colliderB = cB;
		localAnchorA = cA->getLocalPoint(anchor);
		localAnchorB = cB->getLocalPoint(anchor);
		referenceAngle = cB->getAngle() - cA->getAngle();
	}
}
