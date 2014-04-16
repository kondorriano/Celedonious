#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "commons.hpp"

class Player : public GameObject {
	public:
		Player();
		~Player();

		void update(float deltaTime);
		void draw() const;

	private:
};

#endif // PLAYER_HPP
