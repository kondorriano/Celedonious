#ifndef SQUAREOBJECT_HPP
#define SQUAREOBJECT_HPP
#include "commons.hpp"
#include "physics/Physics.hpp"

class DeferredContainer;
class GenericBody : public Physics::Body {
	public:
		GenericBody();
		~GenericBody();

		void update(float deltaTime);
		void set(Physics::Collider* newCollider);

	public:
		Physics::Collider* col;
};

#endif // SQUAREOBJECT_HPP
