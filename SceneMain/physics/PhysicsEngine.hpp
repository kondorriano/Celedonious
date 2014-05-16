#ifndef PHYSICSENGINE_HPP
#define PHYSICSENGINE_HPP
#include "commons.hpp"

class PhysicsEngine : public GameObject, public b2ContactListener {
	public:
		PhysicsEngine(vec2f gravity = vec2f(0.0f, 9.8f), float timestep = 60.0f, int velIters = 2, int posIters = 8);
		~PhysicsEngine();

		void update(float deltaTime);
		void draw() const;

	private:
		b2Body* createBody(b2BodyDef* definition);
		void deleteBody(b2Body* body);

		b2World world;
		const float timestep;
		const int velocityIterations;
		const int positionIterations;

		friend class PhysicsBody;
};

#endif // PHYSICSENGINE_HPP
