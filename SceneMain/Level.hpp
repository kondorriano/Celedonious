#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "commons.hpp"
#include "physics/PhysicsBody.hpp"

class DeferredContainer;
class Level : public PhysicsBody {
	public:
		Level();
		~Level();

		void update(float deltaTime);
		void draw() const;

	private:
		Model quad;
		DeferredContainer* renderer;
};
#endif // LEVEL_HPP
