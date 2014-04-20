#ifndef PHYSICSBODY_HPP
#define PHYSICSBODY_HPP
#include "commons.hpp"

class PhysicsBody : public GameObject {
	public:
		PhysicsBody();
		~PhysicsBody();

		void update(float deltaTime);
		void draw() const;

	private:
};

#endif // PHYSICSBODY_HPP
