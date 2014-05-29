#include "PhysicsEngine.hpp"
#include "PhysicsDebugDrawer.hpp"

b2World* PhysicsEngine::world = nullptr;
float PhysicsEngine::timestep = 0.0f;
int PhysicsEngine::velocityIterations = 0;
int PhysicsEngine::positionIterations = 0;
float PhysicsEngine::accumulated = 0.0f;

void PhysicsEngine::init(vec2f gravity, float step, int velIters, int posIters) {
	VBE_ASSERT(world == nullptr, "Trying to init an already init physics simulation");
	world = new b2World(b2Vec2(gravity.x, gravity.y));
	timestep = 1.0f/step;
	velocityIterations = velIters;
	positionIterations = posIters;
	accumulated = 0.0f;
}

void PhysicsEngine::close() {
	VBE_ASSERT(world != nullptr, "Trying to close a not-init physics simulation");
	delete world;
	world = nullptr;
}

void PhysicsEngine::update(float deltaTime) {
	VBE_ASSERT(world != nullptr, "Trying to update an not-init physics simulation");
	accumulated += deltaTime;
	while(accumulated >= timestep) {
		world->Step(timestep,velocityIterations,positionIterations);
		accumulated -= timestep;
	}
}

void PhysicsEngine::draw(const PhysicsDebugDrawer* drawer) {
	VBE_ASSERT(world != nullptr, "Trying to update an not-init physics simulation");
	world->SetDebugDraw((PhysicsDebugDrawer*)drawer);
	world->DrawDebugData();
}

b2Body* PhysicsEngine::createBody() {
	VBE_ASSERT(world != nullptr, "Trying to add collider to an not-init physics simulation");
	b2BodyDef b;
	return world->CreateBody(&b);
}

void PhysicsEngine::deleteBody(b2Body* body) {
	VBE_ASSERT(world != nullptr, "Trying to remove collider from an not-init physics simulation");
	world->DestroyBody(body);
}
