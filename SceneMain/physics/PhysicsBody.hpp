#ifndef PHYSICSBODY_HPP
#define PHYSICSBODY_HPP
#include "commons.hpp"

class Shape {
	public:
		enum Type {
				Edge = 0,
				Chain = 1,
				Circle = 2,
				Polygon = 3
		};

		Shape();
		virtual ~Shape();
		Type getType() const = 0;

	private:
		b2Shape* shape;
		friend class Component;
};

class Component {
	public:
		Component(const Shape* shape);
		~Component();

	private:
		float friction;
		float restitution;
		float density;
		bool isSensor;

		Shape shape;
		b2Fixture* fixture;

		friend class PhysicsBody;
};

class PhysicsBody : public GameObject, public b2ContactListener {
	public:
		PhysicsBody(const std::vector<Component>& comp);
		virtual ~PhysicsBody();

		void update(float deltaTime);

		virtual void onCollisionEnter(PhysicsBody* other, int compIndex);
		virtual void onCollisionExit(PhysicsBody* other, int compIndex);
		virtual bool onCollision(PhysicsBody* other, int compIndex);

		vec2f getCenter() const;
		vec2f getShape() const;
	private:
		b2Body* body;
};

#endif // PHYSICSBODY_HPP
