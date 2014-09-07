#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "commons.hpp"
#include "physics/Physics.hpp"

class DeferredContainer;
class Player : public Physics::Body {
	public:
		Player();
		~Player();

		void update(float deltaTime);
		void draw() const;

	private:
		Model model;
		Camera* cam;
		vec3f pos;
		Physics::CircleCollider* rueda;
		DeferredContainer* renderer;
};

#endif // PLAYER_HPP
