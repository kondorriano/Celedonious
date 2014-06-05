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
			Chain = b2Shape::e_chain
		};

		Collider();
		virtual ~Collider();

		void applyForce(const vec2f& f, const vec2f& p);
		void applyForceToCenterOfMass(const vec2f& f);
		void applyTorque(float torque);
		void applyLinearImpulse(const vec2f &i, const vec2f &p);
		void applyAngularImpulse(float impulse);

		virtual AABB getAABB() const = 0;
		float getFriction() const {return friction;}
		virtual void setFriction(float friction) = 0;
		float getRestitution() const {return restitution;}
		virtual void setRestitution(float restitution) = 0;
		float getDensity() const {return density;}
		virtual void setDensity(float density) = 0;
		bool isSensor() const {return sensor;}
		virtual void setSensor(bool isSensor) = 0;
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
		DType B2TypeToDType(b2BodyType t) const;
		b2BodyType DTypeToB2Type(DType t) const;

		b2Body* pBody;

		float friction;
		float restitution;
		float density;
		bool sensor;
};

#endif // COLLIDER_HPP
