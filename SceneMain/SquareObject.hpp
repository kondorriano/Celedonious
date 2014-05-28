#ifndef SQUAREOBJECT_HPP
#define SQUAREOBJECT_HPP
#include "commons.hpp"
#include "physics/PhysicsBody.hpp"

class DeferredContainer;
class SquareObject : public PhysicsBody {
	public:
		SquareObject();
		~SquareObject();

		void update(float deltaTime);
		void draw() const;

	private:
		Model quad;
		DeferredContainer* renderer;
};

#endif // SQUAREOBJECT_HPP
