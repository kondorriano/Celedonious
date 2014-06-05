#ifndef PHYSICSBODY_HPP
#define PHYSICSBODY_HPP
#include "commons.hpp"
#include "Collider.hpp"

class PhysicsBody : public GameObject {
	public:
		PhysicsBody();
		virtual ~PhysicsBody();

		virtual void update(float deltaTime) {(void) deltaTime;}
		virtual void draw() const {}
		Collider* getCollider() const {return collider;}

	protected:
		void setCollider(Collider* newCollider);

		virtual void onCollisionEnter(PhysicsBody* other) {(void) other;}
		virtual void onCollisionExit(PhysicsBody* other) {(void) other;}
		virtual void onCollision(PhysicsBody* other) {(void) other;}

	private:
		Collider* collider;
};

#endif // PHYSICSBODY_HPP
