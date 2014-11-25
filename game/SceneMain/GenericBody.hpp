#ifndef SQUAREOBJECT_HPP
#define SQUAREOBJECT_HPP
#include "commons.hpp"

class DeferredContainer;
class GenericBody : public Physics::Body {
	public:
		GenericBody();
		~GenericBody();

		void addCol(Physics::Collider* newCollider);
		Physics::Collider* getCol(unsigned int index) const;
		void deleteCol(unsigned int index);

		void update(float deltaTime) override;
};

#endif // SQUAREOBJECT_HPP
