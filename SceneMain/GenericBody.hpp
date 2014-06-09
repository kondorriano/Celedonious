#ifndef SQUAREOBJECT_HPP
#define SQUAREOBJECT_HPP
#include "commons.hpp"
#include "physics/PhysicsBody.hpp"

class DeferredContainer;
class GenericBody : public PhysicsBody {
	public:
		GenericBody();
		~GenericBody();

		void update(float deltaTime);
		void set(Collider *newCollider);

	public:
		Collider* col;
};

#endif // SQUAREOBJECT_HPP
