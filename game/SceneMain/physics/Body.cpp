#include "Body.hpp"
#include "Collider.hpp"

namespace Physics {

	Body::Body() {
	}

	Body::~Body() {
		for(unsigned int i = 0; i < colliders.size(); ++i)
			delete colliders[i];
	}

	int Body::getColliderIndex(const Collider* col) const{
		for(unsigned int i = 0; i < colliders.size(); ++i) if(colliders[i] == col) return i;
		return -1;
	}

	int Body::getNumColliders() const {
		return colliders.size();
	}

	Collider* Body::getCollider(unsigned int index) const {
		VBE_ASSERT((int)index < getNumColliders(), "Collider index out of range");
		return colliders[index];
	}

	void Body::addCollider(Collider* newCollider) {
		newCollider->init(this);
		colliders.push_back(newCollider);
	}

	void Body::deleteCollider(unsigned int index) {
		VBE_ASSERT((int)index < getNumColliders(), "Collider index out of range");
		delete colliders[index];
		colliders.erase(colliders.begin() + index);
	}

	void Body::onCollisionEnter(int colliderIndex, const BodyContact* contact) {
		(void) colliderIndex;
		(void) contact;
	}

	void Body::onCollisionExit(int colliderIndex, const BodyContact* contact) {
		(void) colliderIndex;
		(void) contact;
	}

	void Body::onCollision(int colliderIndex, const BodyContact* contact) {
		(void) colliderIndex;
		(void) contact;
	}

	void Body::onParticleCollisionEnter(const ParticleBodyContact* contact) {
		(void) contact;
	}

	void Body::onParticleCollisionExit(ParticleSystem* system, int particleIndex) {
		(void) particleIndex;
		(void) system;
	}

}
