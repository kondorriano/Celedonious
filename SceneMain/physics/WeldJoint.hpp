#ifndef WELDJOINT_HPP
#define WELDJOINT_HPP
#include "Joint.hpp"

namespace Physics {

	struct WeldJointDef : public JointDef {
			WeldJointDef() {
				localAnchorA = vec2f(0.0f);
				localAnchorB = vec2f(0.0f);
				referenceAngle = 0.0f;
				frequencyHz = 0.0f;
				dampingRatio = 0.0f;
				colliderA = nullptr;
				colliderB = nullptr;
				collideConnected = false;
			}

			vec2f localAnchorA;
			vec2f localAnchorB;
			float referenceAngle;
			float frequencyHz;
			float dampingRatio;
	};

	class WeldJoint : public Joint {
		public:
			WeldJoint(const WeldJointDef& def);
			~WeldJoint();

			vec2f getAnchorA() const;
			vec2f getAnchorB() const;
			vec2f getReactionForce(float inverseDelta) const;
			float getReactionTorque(float inverseDelta) const;
			float getFrequency() const;
			void setFrequency(float newFrequency);
			float getDampingRatio() const;
			void setDampingRatio(float newDampingRatio);
			float getReferenceAngle() const;
	};

}

#endif // WELDJOINT_HPP
