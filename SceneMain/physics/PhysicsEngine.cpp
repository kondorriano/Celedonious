#include "PhysicsEngine.hpp"

PhysicsEngine::PhysicsEngine(vec2f gravity, float timestep, int velIters, int posIters) :
	world(b2Vec2(gravity.x, gravity.y)),
	timestep(timestep),
	velocityIterations(velIters),
	positionIterations(posIters) {
	setName("physicsEngine");
}

PhysicsEngine::~PhysicsEngine() {
}

void PhysicsEngine::update(float deltaTime) {
	while(deltaTime >= timestep) {
		world.Step(timestep,velocityIterations,positionIterations);
		deltaTime -= timestep;
	}
}

void PhysicsEngine::draw() const {
}

b2Body* PhysicsEngine::createBody(b2BodyDef* definition) {
	return world.CreateBody(definition);
}

void PhysicsEngine::deleteBody(b2Body* body) {
	world.DestroyBody(body);
}
