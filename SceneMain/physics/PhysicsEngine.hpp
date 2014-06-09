#ifndef PHYSICSENGINE_HPP
#define PHYSICSENGINE_HPP
#include "commons.hpp"

class PhysicsBody;
class PhysicsDebugDrawer;
class Collider;
class PhysicsQueryCallback {
	public:
		PhysicsQueryCallback();
		virtual ~PhysicsQueryCallback();

		virtual bool reportCollider(Collider* collider) = 0;
};

class PhysicsEngine {
	public:
		static void init(vec2f gravity = vec2f(0.0f, -9.8f), float step = 60.0f, int velIters = 2, int posIters = 8);
		static void update(float deltaTime);
		static void draw(const PhysicsDebugDrawer* drawer);
		static void drawGrid(const PhysicsDebugDrawer* drawer, vec2f min, vec2f max, float step, vec4f color);
		static void queryAABB(PhysicsQueryCallback* callback, AABB aabb);
		static void close();

		static b2World* world;
	private:
		class PECallback : public b2QueryCallback {
			public:
				PECallback();
				~PECallback();
				bool ReportFixture(b2Fixture *fixture);
				PhysicsQueryCallback* callback;

				void reset(PhysicsQueryCallback* newCallback);
		};

		static b2Body* createBody();
		static void deleteBody(b2Body* body);

		static float timestep;
		static int velocityIterations;
		static int positionIterations;
		static float accumulated;
		static PECallback queryCallback;

		friend class Collider;

		PhysicsEngine() {}
		~PhysicsEngine() {}

};

#endif // PHYSICSENGINE_HPP
