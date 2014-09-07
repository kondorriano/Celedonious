#include "WeldJoint.hpp"
#include "Collider.hpp"
#include "Engine.hpp"

namespace Physics {

	WeldJoint::WeldJoint(const WeldJointDef& def) {
		cA = def.colliderA;
		cB = def.colliderB;
		cA->joints.push_back(this);
		cB->joints.push_back(this);
		b2WeldJointDef d;
		d.bodyA = cA->pBody;
		d.bodyB = cB->pBody;
		d.dampingRatio = def.dampingRatio;
		d.frequencyHz = def.frequencyHz;
		d.referenceAngle = def.referenceAngle;
		d.collideConnected = def.collideConnected;
		d.localAnchorA = Utils::GLMv2ToB2Dv2(def.localAnchorA);
		d.localAnchorB = Utils::GLMv2ToB2Dv2(def.localAnchorB);
		joint = Engine::createJoint(&d);
		joint->SetUserData(this);
	}

	WeldJoint::~WeldJoint() {
	}

	vec2f WeldJoint::getAnchorA() const {
		return Utils::B2Dv2ToGLMv2(((b2WeldJoint*)joint)->GetAnchorA());
	}

	vec2f WeldJoint::getAnchorB() const {
		return Utils::B2Dv2ToGLMv2(((b2WeldJoint*)joint)->GetAnchorB());
	}

	vec2f WeldJoint::getReactionForce(float inverseDelta) const {
		return Utils::B2Dv2ToGLMv2(((b2WeldJoint*)joint)->GetReactionForce(inverseDelta));
	}

	float WeldJoint::getReactionTorque(float inverseDelta) const {
		return ((b2WeldJoint*)joint)->GetReactionTorque(inverseDelta);
	}

	float WeldJoint::getFrequency() const {
		return ((b2WeldJoint*)joint)->GetFrequency();
	}

	void WeldJoint::setFrequency(float newFrequency) {
		((b2WeldJoint*)joint)->SetFrequency(newFrequency);
	}

	float WeldJoint::getDampingRatio() const {
		return ((b2WeldJoint*)joint)->GetDampingRatio();
	}

	void WeldJoint::setDampingRatio(float newDampingRatio) {
		((b2WeldJoint*)joint)->SetDampingRatio(newDampingRatio);
	}

	float WeldJoint::getReferenceAngle() const {
		return ((b2WeldJoint*)joint)->GetReferenceAngle();
	}

}
