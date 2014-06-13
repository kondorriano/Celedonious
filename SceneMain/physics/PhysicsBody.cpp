#include "PhysicsBody.hpp"

PhysicsBody::PhysicsBody() {
}

PhysicsBody::~PhysicsBody() {
	for(unsigned int i = 0; i < colliders.size(); ++i)
		delete colliders[i];
}

void PhysicsBody::addCollider(Collider* newCollider) {
	newCollider->init(this);
	colliders.push_back(newCollider);
}

int PhysicsBody::getNumColliders() const {
	return colliders.size();
}

Collider*PhysicsBody::getCollider(int index) const {
	VBE_ASSERT(index < getNumColliders(), "Collider index out of range");
	return colliders[index];
}

void PhysicsBody::onCollisionEnter(int colliderIndex, Collider* other) {
	(void) colliderIndex;
	(void) other;
}

void PhysicsBody::onCollisionExit(int colliderIndex, Collider* other) {
	(void) colliderIndex;
	(void) other;
}

void PhysicsBody::onCollision(int colliderIndex, Collider* other) {
	(void) colliderIndex;
	(void) other;
}

