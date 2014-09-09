#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "commons.hpp"
#include "physics/Physics.hpp"

class DeferredContainer;
class Player : public Physics::Body {
	public:

		enum ColliderID {
			Wheel = 0,
			Body = 1,
			Axis = 2
		};

		Player();
		~Player();

		void update(float deltaTime);
		void draw() const;

	private:
		Model model;
		Camera* cam;
		vec3f pos;
		DeferredContainer* renderer;
};

#endif // PLAYER_HPP
