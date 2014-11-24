#ifndef JOINT_HPP
#define JOINT_HPP
#include "commons.hpp"

namespace Physics {

	class Collider;
	struct JointDef {
			JointDef(){}
			virtual ~JointDef(){}

			Collider* colliderA;
			Collider* colliderB;
			bool collideConnected;
	};

	class Joint {
		public:
			enum Type {
				Unknown = e_unknownJoint,
				Revolute = e_revoluteJoint,
				Prismatic = e_prismaticJoint,
				Distance = e_distanceJoint,
				Pulley = e_pulleyJoint,
				Mouse = e_mouseJoint,
				Gear = e_gearJoint,
				Wheel = e_wheelJoint,
				Weld = e_weldJoint,
				Friction = e_frictionJoint,
				Rope = e_ropeJoint,
				Motor = e_motorJoint
			};

			Joint(Type type);
			virtual ~Joint();

			Collider* getColliderA() {return cA;}
			Collider* getColliderB() {return cB;}

			virtual vec2f getAnchorA() const = 0;
			virtual vec2f getAnchorB() const = 0;

			virtual vec2f getReactionForce(float inverseDelta) const = 0;
			virtual float getReactionTorque(float inverseDelta) const = 0;

			bool isActive() const;
			bool getCollideConnected() const;

			Type getType() const {return type;}

		protected:
			friend class Collider;

			Collider* cA;
			Collider* cB;

			b2Joint* joint;

		private:
			Type type;
	};

}

#endif // JOINT_HPP
