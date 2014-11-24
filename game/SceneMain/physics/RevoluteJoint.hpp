#ifndef REVOLUTEJOINT_HPP
#define REVOLUTEJOINT_HPP
#include "Joint.hpp"

namespace Physics {

	struct RevoluteJointDef final : public JointDef {
			RevoluteJointDef() {
				colliderA = nullptr;
				colliderB = nullptr;
				collideConnected = false;
				localAnchorA = vec2f(0.0f);
				localAnchorB = vec2f(0.0f);
				referenceAngle = 0.0f;
				frequencyHz = 0.0f;
				dampingRatio = 0.0f;
				enableLimit = false;
				enableMotor = false;
				lowerAngle = 0.0f;
				upperAngle = 0.0f;
				maxMotorTorque = 0.0f;
				motorSpeed = 0.0f;
			}

			vec2f localAnchorA;
			vec2f localAnchorB;
			float referenceAngle;
			float frequencyHz;
			float dampingRatio;
			bool enableLimit;
			bool enableMotor;
			float lowerAngle;
			float upperAngle;
			float maxMotorTorque;
			float motorSpeed;

			void init(Collider* cA, Collider* cB, const vec2f& anchor);
	};

	class RevoluteJoint final : public Joint {
		public:
			RevoluteJoint(const RevoluteJointDef& def);
			~RevoluteJoint() override;

			vec2f getAnchorA() const override;
			vec2f getAnchorB() const override;
			vec2f getLocalAnchorA() const;
			vec2f getLocalAnchorB() const;
			vec2f getReactionForce(float inverseDelta) const override;
			float getReactionTorque(float inverseDelta) const override;
			float getJointAngle() const;
			float getJointSpeed() const;
			float getLowerLimit() const;
			float getUpperLimit() const;
			float getMaxMotorTorque() const;
			float getMotorSpeed() const;
			float getMotorTorque(float inverseDeltatime) const;
			float getReferenceAngle() const;

			void setLimits(float lower, float upper) const;
			void setMaxMotorTorque(float torque) const;
			void setMotorSpeed(float speed) const;

			void enableLimit(bool flag) const;
			void enableMotor(bool flag) const;

			bool isLimitEnabled() const;
			bool isMotorEnabled() const;
	};

}

#endif // REVOLUTEJOINT_HPP
