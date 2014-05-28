#include "PhysicsBody.hpp"

PhysicsBody::PhysicsBody() : collider(nullptr) {
}

PhysicsBody::~PhysicsBody() {
}

void PhysicsBody::setCollider(Collider* newCollider) {
	if(collider != nullptr) delete collider;
	collider = newCollider;
	collider->init(this);
}

