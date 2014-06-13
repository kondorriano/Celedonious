#ifndef COLLIDER_HPP
#define COLLIDER_HPP
#include "commons.hpp"

class PhysicsEngine;
class PhysicsBody;
class Collider {
	friend class PhysicsBody;

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
			Undefined = -1
		};

		Collider();
		virtual ~Collider();

		PhysicsBody* getBody();

		void applyForce(const vec2f& f, const vec2f& p);
		void applyForceToCenterOfMass(const vec2f& f);
		void applyTorque(float torque);
		void applyLinearImpulse(const vec2f &i, const vec2f &p);
		void applyAngularImpulse(float impulse);

		bool testPoint(vec2f point);

		CType getCType() {return type;}
		AABB getAABB() const;
		float getFriction() const {return friction;}
		void setFriction(float friction);
		float getRestitution() const {return restitution;}
		void setRestitution(float restitution);
		float getDensity() const {return density;}
		void setDensity(float density);
		bool isSensor() const {return sensor;}
		void setSensor(bool isSensor);
		vec2f getCenterOfMass() const;
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
		bool isActive() const;
		void setActive(bool b);
		float getInertia() const;
		float getMass() const;

	protected:
		void init(PhysicsBody* pb);
		void remake();
		DType B2TypeToDType(b2BodyType t) const;
		b2BodyType DTypeToB2Type(DType t) const;

		b2Body* pBody;
		b2Fixture* fixture;
		b2Shape* shape;
		PhysicsBody* node;
		CType type;

		float friction;
		float restitution;
		float density;
		bool sensor;
};

#endif // COLLIDER_HPP
