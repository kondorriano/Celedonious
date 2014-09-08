#ifndef COLLIDER_HPP
#define COLLIDER_HPP
#include "commons.hpp"

namespace Physics {

	class Engine;
	class Body;
	class Joint;
	class Collider {
			friend class Body;

		public:
			enum DType {
				Dynamic = b2_dynamicBody,
				Static = b2_staticBody,
				Kinematic = b2_kinematicBody
			};

			enum CType {
				Circle = b2Shape::e_circle,
				Polygon = b2Shape::e_polygon,
				Edge = b2Shape::e_edge,
				Chain = b2Shape::e_chain,
				Undefined = b2Shape::e_typeCount
			};

			Collider();
			virtual ~Collider();

			Body* getBody();
			const Body* getBody() const;
			Joint* getJoint(unsigned int index);
			const Joint* getJoint(unsigned int index) const;
			void deleteJoint(unsigned int index);
			int getJointCount() const;
			int getJointIndex(const Joint* joint) const;

			void applyForce(const vec2f& f, const vec2f& p);
			void applyForceToCenterOfMass(const vec2f& f);
			void applyTorque(float torque);
			void applyLinearImpulse(const vec2f &i, const vec2f &p);
			void applyAngularImpulse(float impulse);
			bool testPoint(vec2f point);

			std::pair<float, vec2f> computeDistance(const vec2f& p) const;
			//bool RayCast(b2RayCastOutput* output, const b2RayCastInput& input, int32 childIndex) const;

			CType getCType() {return type;}
			AABB getAABB() const;

			vec2f getWorldPoint(const vec2f& localPoint) const;
			vec2f getWorldVector(const vec2f& localVector) const;
			vec2f getLocalPoint(const vec2f& worldPoint) const;
			vec2f getLocalVector(const vec2f& worldVector) const;
			vec2f getLinearVelocityFromWorldPoint(const vec2f& worldPoint) const;
			vec2f getLinearVelocityFromLocalPoint(const vec2f& localPoint) const;

			float getFriction() const {return friction;}
			void setFriction(float friction);

			float getRestitution() const {return restitution;}
			void setRestitution(float restitution);

			float getDensity() const {return density;}
			void setDensity(float density);

			bool isSensor() const {return sensor;}
			void setSensor(bool isSensor);

			float getRadius() const;
			void setRadius(float r);

			vec2f getWorldCenterOfMass() const;
			vec2f GetLocalCenterOfMass() const;

			vec2f getPosition() const;
			void setPosition(vec2f pos);


			float getRotation() const;
			void setRotation(float angle);

			void setTransform(const vec2f& pos, float angle);

			vec2f getLinearVelocity() const;
			void setLinearVelocity(const vec2f& lv);

			float getAngularVelocity() const;
			void setAngularVelocity(float v);

			float getLinearDamping() const;
			void setLinearDamping(float damping);

			float getAngularDamping() const;
			void setAngularDamping(float damping);

			float getGravityScale() const;
			void setGravityScale(float scale);

			DType getDType() const;
			void setDType(DType t);

			bool isBullet() const;
			void setBullet(bool b);

			bool isFixedRotation() const;
			void setFixedRotation(bool b);

			bool isSleepingAllowed() const;
			void setSleepingAllowed(bool flag);

			bool isAwake() const;
			void setAwake(bool b);

			bool isActive() const;
			void setActive(bool b);

			float getInertia() const;
			float getMass() const;
			float getAngle() const;

		protected:
			friend class WeldJoint;
			friend class DistanceJoint;
			friend class RevoluteJoint;

			void init(Body* pb);
			void remake(b2Shape* newShape);

			b2Body* pBody;
			b2Fixture* fixture;
			b2Shape* shape;
			Body* node;
			CType type;

			float friction;
			float restitution;
			float density;
			bool sensor;

			std::vector<Joint*> joints;
	};

}

#endif // COLLIDER_HPP
