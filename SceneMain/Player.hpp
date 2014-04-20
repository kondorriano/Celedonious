#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "commons.hpp"

class DeferredContainer;
class Player : public GameObject {
	public:
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
