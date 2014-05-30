#ifndef PHYSICSENGINE_HPP
#define PHYSICSENGINE_HPP
#include "commons.hpp"

class PhysicsDebugDrawer;
class PhysicsEngine : public b2ContactListener {
	public:
		static void init(vec2f gravity = vec2f(0.0f, -9.8f), float step = 60.0f, int velIters = 2, int posIters = 8);
		static void update(float deltaTime);
		static void draw(const PhysicsDebugDrawer* drawer);
		static void drawGrid(const PhysicsDebugDrawer* drawer);
		static void close();

	private:
		static b2Body* createBody();
		static void deleteBody(b2Body* body);

		static b2World* world;
		static float timestep;
		static int velocityIterations;
		static int positionIterations;
		static float accumulated;

		friend class Collider;

		PhysicsEngine() {}
		~PhysicsEngine() {}

};

#endif // PHYSICSENGINE_HPP
