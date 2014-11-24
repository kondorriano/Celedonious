#include "GenericBody.hpp"
#include "DeferredContainer.hpp"

GenericBody::GenericBody() {
}

GenericBody::~GenericBody() {
}

void GenericBody::update(float deltaTime) {
	(void) deltaTime;
	transform = glm::translate(mat4f(1.0f), vec3f(col->getPosition(), 0.0f));
	transform = glm::rotate(transform, glm::degrees(col->getRotation()), vec3f(0, 0, 1));
}

void GenericBody::set(Physics::Collider* newCollider) {
	col = newCollider;
	addCollider(newCollider);
}
