#ifndef WELDJOINT_HPP
#define WELDJOINT_HPP
#include "Joint.hpp"

namespace Physics {

	struct WeldJointDef final : public JointDef {
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

			void init(Collider* cA, Collider* cB, const vec2f& anchor);

			vec2f localAnchorA;
			vec2f localAnchorB;
			float referenceAngle;
			float frequencyHz;
			float dampingRatio;
	};

	class WeldJoint final : public Joint {
		public:
			WeldJoint(const WeldJointDef& def);
			~WeldJoint() override;

			vec2f getAnchorA() const override;
			vec2f getAnchorB() const override;
			vec2f getLocalAnchorA() const;
			vec2f getLocalAnchorB() const;
			vec2f getReactionForce(float inverseDelta) const override;
			float getReactionTorque(float inverseDelta) const override;
			float getFrequency() const;
			void setFrequency(float newFrequency);
			float getDampingRatio() const;
			void setDampingRatio(float newDampingRatio);
			float getReferenceAngle() const;
	};

}

#endif // WELDJOINT_HPP
