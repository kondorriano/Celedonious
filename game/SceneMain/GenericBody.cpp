#include "GenericBody.hpp"
#include "DeferredContainer.hpp"

GenericBody::GenericBody() {
}

GenericBody::~GenericBody() {
}

void GenericBody::addCol(Physics::Collider* newCollider) {
	Physics::Body::addCollider(newCollider);
}

Physics::Collider* GenericBody::getCol(unsigned int index) const {
	return Physics::Body::getCollider(index);
}

void GenericBody::deleteCol(unsigned int index) {
	Physics::Body::deleteCollider(index);
}

void GenericBody::update(float deltaTime) {
	(void) deltaTime;
	transform = glm::translate(mat4f(1.0f), vec3f(getCol(0)->getPosition(), 0.0f));
	transform = glm::rotate(transform, glm::degrees(getCol(0)->getRotation()), vec3f(0, 0, 1));
}
