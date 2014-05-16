#include "PhysicsBody.hpp"

PhysicsBody::PhysicsBody(const std::vector<Component>& comp){
}

PhysicsBody::~PhysicsBody() {
}

void PhysicsBody::update(float deltaTime) {
}

void PhysicsBody::onCollisionEnter(PhysicsBody* other, int compIndex) {
}

void PhysicsBody::onCollisionExit(PhysicsBody* other, int compIndex) {
}

bool PhysicsBody::onCollision(PhysicsBody* other, int compIndex) {
}

void PhysicsBody::draw() const {
}
