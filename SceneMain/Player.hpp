#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "commons.hpp"
#include "physics/PhysicsBody.hpp"

class DeferredContainer;
class CircleCollider;
class Player : public PhysicsBody {
	public:
		Player();
		~Player();

		void update(float deltaTime);
		void draw() const;

	private:
		Model model;
		Camera* cam;
		vec3f pos;
		CircleCollider* rueda;
		DeferredContainer* renderer;
};

#endif // PLAYER_HPP
