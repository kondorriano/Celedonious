#include "PhysicsBody.hpp"

PhysicsBody::PhysicsBody() {
}

PhysicsBody::~PhysicsBody() {
	for(int i = 0; i < colliders.size(); ++i)
		delete colliders[i];
}

void PhysicsBody::addCollider(Collider* newCollider) {
	newCollider->init(this);
	colliders.push_back(newCollider);
}

