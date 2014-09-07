#ifndef LEVEL_HPP
#define LEVEL_HPP
#include "commons.hpp"
#include "physics/Physics.hpp"

class DeferredContainer;
class Level : public Physics::Body {
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
