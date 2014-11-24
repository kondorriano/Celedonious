#include "DistanceJoint.hpp"
#include "Engine.hpp"
#include "Collider.hpp"

namespace Physics {

	DistanceJoint::DistanceJoint(const DistanceJointDef& def) {
		cA = def.colliderA;
		cB = def.colliderB;
		cA->joints.push_back(this);
		cB->joints.push_back(this);
		b2DistanceJointDef d;
		d.bodyA = cA->pBody;
		d.bodyB = cB->pBody;
		d.dampingRatio = def.dampingRatio;
		d.frequencyHz = def.frequencyHz;
		d.length = def.length;
		d.collideConnected = def.collideConnected;
		d.localAnchorA = Utils::GLMv2ToB2Dv2(def.localAnchorA);
		d.localAnchorB = Utils::GLMv2ToB2Dv2(def.localAnchorB);
		d.userData = this;
		joint = Engine::createJoint(&d);
	}

	DistanceJoint::~DistanceJoint() {
	}

	vec2f DistanceJoint::getAnchorA() const {
		return Utils::B2Dv2ToGLMv2(((b2DistanceJoint*)joint)->GetAnchorA());
	}

	vec2f DistanceJoint::getAnchorB() const {
		return Utils::B2Dv2ToGLMv2(((b2DistanceJoint*)joint)->GetAnchorB());
	}

	vec2f DistanceJoint::getLocalAnchorA() const {
		return Utils::B2Dv2ToGLMv2(((b2DistanceJoint*)joint)->GetLocalAnchorA());
	}

	vec2f DistanceJoint::getLocalAnchorB() const {
		return Utils::B2Dv2ToGLMv2(((b2DistanceJoint*)joint)->GetLocalAnchorB());
	}

	vec2f DistanceJoint::getReactionForce(float inverseDelta) const {
		return Utils::B2Dv2ToGLMv2(((b2DistanceJoint*)joint)->GetReactionForce(inverseDelta));
	}

	float DistanceJoint::getReactionTorque(float inverseDelta) const {
		return ((b2DistanceJoint*)joint)->GetReactionTorque(inverseDelta);
	}

	float DistanceJoint::getFrequency() const {
		return ((b2DistanceJoint*)joint)->GetFrequency();
	}

	void DistanceJoint::setFrequency(float newFrequency) {
		((b2DistanceJoint*)joint)->SetFrequency(newFrequency);
	}

	float DistanceJoint::getDampingRatio() const {
		return ((b2DistanceJoint*)joint)->GetDampingRatio();
	}

	void DistanceJoint::setDampingRatio(float newDampingRatio) {
		((b2DistanceJoint*)joint)->SetDampingRatio(newDampingRatio);
	}

	float DistanceJoint::getLength() const {
		return ((b2DistanceJoint*)joint)->GetLength();
	}

	void DistanceJoint::setLength(float newLength) {
		((b2DistanceJoint*)joint)->SetLength(newLength);
	}

	void DistanceJointDef::init(Collider* cA, Collider* cB, const vec2f& anchor1, const vec2f& anchor2) {
		colliderA = cA;
		colliderB = cB;
		localAnchorA = cA->getLocalPoint(anchor1);
		localAnchorB = cB->getLocalPoint(anchor2);
		vec2f d = anchor2 - anchor1;
		length = glm::length(d);
	}

}
