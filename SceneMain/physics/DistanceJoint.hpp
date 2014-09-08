#ifndef DISTANCEJOINT_HPP
#define DISTANCEJOINT_HPP
#include "Joint.hpp"

namespace Physics {

	struct DistanceJointDef : public JointDef {
			DistanceJointDef() {
				colliderA = nullptr;
				colliderB = nullptr;
				collideConnected = false;
				localAnchorA = vec2f(0.0f);
				localAnchorB = vec2f(0.0f);
				length = 1.0f;
				frequencyHz = 0.0f;
				dampingRatio = 0.0f;
			}

			void init(Collider* cA, Collider* cB, const vec2f& anchor1, const vec2f& anchor2);

			vec2f localAnchorA;
			vec2f localAnchorB;
			float length;
			float frequencyHz;
			float dampingRatio;
	};

	class DistanceJoint : public Joint {
		public:
			DistanceJoint(const DistanceJointDef& def);
			~DistanceJoint();

			vec2f getAnchorA() const;
			vec2f getAnchorB() const;
			vec2f getLocalAnchorA() const;
			vec2f getLocalAnchorB() const;
			vec2f getReactionForce(float inverseDelta) const;
			float getReactionTorque(float inverseDelta) const;
			void setFrequency(float newFrequency);
			float getFrequency() const;
			void setDampingRatio(float newDampingRatio);
			float getDampingRatio() const;
			void setLength(float newLength);
			float getLength() const;
	};

}

#endif // DISTANCEJOINT_HPP
