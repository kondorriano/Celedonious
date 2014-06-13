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

	protected:
		void addCollider(Collider* newCollider);
		int getNumColliders() const;
		Collider* getCollider(int index) const;

		virtual void onCollisionEnter(int colliderIndex, Collider* other);
		virtual void onCollisionExit(int colliderIndex, Collider* other);
		virtual void onCollision(int colliderIndex, Collider* other);

	private:
		std::vector<Collider*> colliders;
};

#endif // PHYSICSBODY_HPP
