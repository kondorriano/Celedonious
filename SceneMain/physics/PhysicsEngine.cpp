#include "PhysicsEngine.hpp"
#include "PhysicsDebugDrawer.hpp"

b2World* PhysicsEngine::world = nullptr;
float PhysicsEngine::timestep = 0.0f;
int PhysicsEngine::velocityIterations = 0;
int PhysicsEngine::positionIterations = 0;
float PhysicsEngine::accumulated = 0.0f;
PhysicsEngine::PECallback PhysicsEngine::queryCallback;

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

void PhysicsEngine::drawGrid(const PhysicsDebugDrawer* drawer, vec2f min, vec2f max, float step, vec4f color) {
	VBE_ASSERT(step > 0, "DrawGrid()'s step can't be 0 or less");
	for(float i = min.x; i <= max.x; i += step)
		((PhysicsDebugDrawer*)drawer)->drawSegment(vec2f(min.y, i), vec2f(max.y, i), color);
	for(float i = min.y; i <= max.y; i += step)
		((PhysicsDebugDrawer*)drawer)->drawSegment(vec2f(i, min.x), vec2f(i, max.x), color);
}

void PhysicsEngine::queryAABB(PhysicsQueryCallback* callback, AABB aabb) {
	b2AABB b2aabb;
	b2aabb.lowerBound = b2Vec2(aabb.getMin().x, aabb.getMin().y);
	b2aabb.upperBound = b2Vec2(aabb.getMax().x, aabb.getMax().y);
	queryCallback.reset(callback);
	world->QueryAABB(&queryCallback, b2aabb);
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

PhysicsEngine::PECallback::PECallback() {
}

PhysicsEngine::PECallback::~PECallback() {
}

bool PhysicsEngine::PECallback::ReportFixture(b2Fixture* fixture) {
	Collider* collider = (Collider*) fixture->GetBody()->GetUserData();
	VBE_WARN((collider != nullptr), "An orphan collider has showed up on an AABB query. Orphaned colliders cannot be queried on AABB queries");
	return callback->reportCollider(collider);
}

void PhysicsEngine::PECallback::reset(PhysicsQueryCallback* newCallback) {
	callback = newCallback;
}

PhysicsQueryCallback::PhysicsQueryCallback(){
}

PhysicsQueryCallback::~PhysicsQueryCallback(){
}
