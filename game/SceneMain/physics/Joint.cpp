#include "Joint.hpp"
#include "Engine.hpp"
#include "Collider.hpp"

namespace Physics {

	Joint::Joint(Type type) : cA(nullptr), cB(nullptr), joint(nullptr), type(type) {
	}

	Joint::~Joint() {
		Engine::deleteJoint(joint);
	}

	bool Joint::isActive() const {
		return joint->IsActive();
	}

	bool Joint::getCollideConnected() const {
		return joint->GetCollideConnected();
	}

}
