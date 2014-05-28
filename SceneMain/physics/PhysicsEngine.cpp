#include "PhysicsEngine.hpp"

PhysicsEngine::PhysicsEngine(vec2f gravity, float timestep, int velIters, int posIters) :
	world(b2Vec2(gravity.x, gravity.y)),
	timestep(1.0f/timestep),
	velocityIterations(velIters),
	positionIterations(posIters),
	accumulated(0.0f) {
	setName("physicsEngine");
}

PhysicsEngine::~PhysicsEngine() {
}

void PhysicsEngine::update(float deltaTime) {
	accumulated += deltaTime;
	while(accumulated >= timestep) {
		world.Step(timestep,velocityIterations,positionIterations);
		accumulated -= timestep;
	}
}

void PhysicsEngine::draw() const {
}

b2Body* PhysicsEngine::createBody() {
	b2BodyDef b;
	return world.CreateBody(&b);
}

void PhysicsEngine::deleteBody(b2Body* body) {
	world.DestroyBody(body);
}
