#include "Engine.hpp"
#include "DebugDrawer.hpp"
#include "Collider.hpp"
#include "Body.hpp"
#include "ParticleSystem.hpp"

namespace Physics {

	b2World* Engine::world = nullptr;
	float Engine::timestep = 0.0f;
	int Engine::velocityIterations = 0;
	int Engine::positionIterations = 0;
	int Engine::particleIterations = 0;
	float Engine::accumulated = 0.0f;
	Engine::EngineQueryCallback Engine::queryCallback;
	Engine::CollisionCallback Engine::collisionCallback;
	BodyContact* Engine::bodyContactBuffer = nullptr;
	ParticleBodyContact* Engine::particleBodyContactBuffer = nullptr;
	ParticleContact* Engine::particleContactBuffer = nullptr;
	int Engine::bodyContactNum = 0;
	int Engine::particleBodyContactNum = 0;
	int Engine::particleContactNum = 0;
	std::map<const b2ParticleSystem*, ParticleSystem*> Engine::particleSystemBindings;
	std::set<Collider*> Engine::deadBodyList;

	void Engine::init(vec2f gravity, float step, int velIters, int posIters, int particleIters) {
		VBE_ASSERT(world == nullptr, "Trying to init an already init physics simulation");
		world = new b2World(b2Vec2(gravity.x, gravity.y));
		timestep = 1.0f/step;
		velocityIterations = velIters;
		positionIterations = posIters;
		particleIterations = particleIters;
		accumulated = 0.0f;
		bodyContactBuffer = new BodyContact[MAX_CONCURRENT_BODYCONTACTS];
		particleBodyContactBuffer = new ParticleBodyContact[MAX_CONCURRENT_BODYCONTACTS];
		particleContactBuffer = new ParticleContact[MAX_CONCURRENT_PARTICLEBODYCONTACTS];
		world->SetContactListener(&collisionCallback);
	}

	void Engine::close() {
		VBE_ASSERT(world != nullptr, "Trying to close a not-init physics simulation");
		delete world;
		delete[] bodyContactBuffer;
		delete[] particleBodyContactBuffer;
		delete[] particleContactBuffer;
		world = nullptr;
		timestep = 0.0f;
		velocityIterations = 0;
		positionIterations = 0;
		particleIterations = 0;
		accumulated = 0.0f;
		bodyContactNum = 0;
		particleBodyContactNum = 0;
		particleContactNum = 0;
		bodyContactBuffer = nullptr;
		particleBodyContactBuffer = nullptr;
		particleContactBuffer = nullptr;
		particleSystemBindings.clear();
		deadBodyList.clear();
	}

	void Engine::update(float deltaTime) {
		VBE_ASSERT(world != nullptr, "Trying to update an not-init physics simulation");
		accumulated += deltaTime;
		while(accumulated >= timestep) {
			world->Step(timestep, velocityIterations, positionIterations, particleIterations);
			accumulated -= timestep;
			processContacts();
		}
	}

	void Engine::draw(const DebugDrawer* drawer) {
		VBE_ASSERT(world != nullptr, "Trying to update an not-init physics simulation");
		world->SetDebugDraw((DebugDrawer*)drawer);
		world->DrawDebugData();
	}

	void Engine::drawGrid(const DebugDrawer* drawer, vec2f min, vec2f max, float step, vec4f color) {
		VBE_ASSERT(step > 0, "DrawGrid()'s step can't be 0 or less");
		for(float i = min.x; i <= max.x; i += step)
			((DebugDrawer*)drawer)->drawSegment(vec2f(min.y, i), vec2f(max.y, i), color);
		for(float i = min.y; i <= max.y; i += step)
			((DebugDrawer*)drawer)->drawSegment(vec2f(i, min.x), vec2f(i, max.x), color);
	}

	void Engine::queryAABB(QueryCallback* callback, AABB aabb) {
		b2AABB b2aabb;
		b2aabb.lowerBound = b2Vec2(aabb.getMin().x, aabb.getMin().y);
		b2aabb.upperBound = b2Vec2(aabb.getMax().x, aabb.getMax().y);
		queryCallback.reset(callback);
		world->QueryAABB(&queryCallback, b2aabb);
	}

	b2Body* Engine::createBody() {
		VBE_ASSERT(world != nullptr, "Trying to add collider to an not-init physics simulation");
		b2BodyDef b;
		return world->CreateBody(&b);
	}

	void Engine::deleteBody(b2Body* body) {
		VBE_ASSERT(world != nullptr, "Trying to remove collider from an not-init physics simulation");
		deadBodyList.insert((Collider*) body->GetUserData());
		world->DestroyBody(body);
	}

	b2ParticleSystem* Engine::createParticleSystem(ParticleSystem* ppsys) {
		VBE_ASSERT(world != nullptr, "Trying to add system to an not-init physics simulation");
		b2ParticleSystemDef def;
		b2ParticleSystem* sys = world->CreateParticleSystem(&def);
		Engine::particleSystemBindings.insert(std::pair<const b2ParticleSystem*, ParticleSystem*>(sys, ppsys));
		return sys;
	}

	void Engine::deleteParticleSystem(b2ParticleSystem* sys) {
		VBE_ASSERT(world != nullptr, "Trying to remove system from an not-init physics simulation");
		Engine::particleSystemBindings.erase(sys);
		world->DestroyParticleSystem(sys);
	}

	b2Joint*Engine::createJoint(b2JointDef* d) {
		return world->CreateJoint(d);
	}

	void Engine::deleteJoint(b2Joint* j) {
		world->DestroyJoint(j);
	}

	void Engine::registerContact(b2Contact* c, BodyContact::Type t) { //body contact start/end/stay
		VBE_WARN(bodyContactNum < MAX_CONCURRENT_BODYCONTACTS, "Physics engine: Maximum concurrent body contacts limit reached. Please raise limit or prevent contacts");
		if(bodyContactNum == MAX_CONCURRENT_BODYCONTACTS) return;

		const b2Manifold* manifold = c->GetManifold();
		if (manifold->pointCount == 0) return;

		Collider* colliderA = (Collider*) c->GetFixtureA()->GetUserData();
		Collider* colliderB = (Collider*) c->GetFixtureB()->GetUserData();

		b2WorldManifold worldManifold;
		c->GetWorldManifold(&worldManifold);

		BodyContact* cb = &bodyContactBuffer[bodyContactNum];
		cb->a = colliderA;
		cb->b = colliderB;
		cb->normal = Utils::B2Dv2ToGLMv2(worldManifold.normal);
		cb->pointCount = manifold->pointCount;
		cb->type = t;
		for(int i = 0; i < manifold->pointCount; ++i){
			cb->points[i].position = Utils::B2Dv2ToGLMv2(worldManifold.points[i]);
			cb->points[i].normalImpulse = manifold->points[i].normalImpulse;
			cb->points[i].tangentImpulse = manifold->points[i].tangentImpulse;
			cb->points[i].separation = worldManifold.separations[i];
		}
		++bodyContactNum;
	}

	void Engine::registerContact(b2ParticleSystem* particleSystem, b2ParticleBodyContact* particleBodyContact) {//particle-body start
		VBE_WARN(particleBodyContactNum < MAX_CONCURRENT_PARTICLEBODYCONTACTS, "Physics engine: Maximum concurrent particle-body contacts limit reached. Please raise limit or prevent contacts");
		if(particleBodyContactNum == MAX_CONCURRENT_PARTICLEBODYCONTACTS) Log::warning() << "b" << Log::Flush; return;
		ParticleBodyContact* pbc = &particleBodyContactBuffer[particleBodyContactNum];
		pbc->collider = (Collider*) particleBodyContact->fixture->GetUserData();
		pbc->sys = particleSystemBindings.at(particleSystem);
		pbc->mass = particleBodyContact->mass;
		pbc->weight = particleBodyContact->weight;
		pbc->normal = Utils::B2Dv2ToGLMv2(particleBodyContact->normal);
		pbc->particle = particleBodyContact->index;
		pbc->type = ParticleBodyContact::Begin;
		++particleBodyContactNum;
	}

	void Engine::registerContact(b2Fixture* fixture, b2ParticleSystem* particleSystem, int32 index) {//particle-body end
		VBE_WARN(particleBodyContactNum < MAX_CONCURRENT_PARTICLEBODYCONTACTS, "Physics engine: Maximum concurrent particle-body contacts limit reached. Please raise limit or prevent contacts");
		if(particleBodyContactNum == MAX_CONCURRENT_PARTICLEBODYCONTACTS) Log::warning() << "b" << Log::Flush; return;
		ParticleBodyContact* pbc = &particleBodyContactBuffer[particleBodyContactNum];
		pbc->collider = (Collider*) fixture->GetUserData();
		pbc->sys = particleSystemBindings.at(particleSystem);
		pbc->particle = index;
		pbc->type = ParticleBodyContact::End;
		++particleBodyContactNum;
	}

	void Engine::registerContact(b2ParticleSystem* particleSystem, b2ParticleContact* particleContact) {//particle-particle start
		VBE_WARN(particleContactNum < MAX_CONCURRENT_PARTICLECONTACTS, "Physics engine: Maximum concurrent particle contacts limit reached. Please raise limit or prevent contacts");
		if(particleContactNum == MAX_CONCURRENT_PARTICLECONTACTS) Log::warning() << "p" << Log::Flush; return;
		ParticleContact* ppc = &particleContactBuffer[particleBodyContactNum];
		ppc->sys = particleSystemBindings.at(particleSystem);
		ppc->a = particleContact->GetIndexA();
		ppc->b = particleContact->GetIndexB();
		ppc->weight = particleContact->GetWeight();
		ppc->normal = Utils::B2Dv2ToGLMv2(particleContact->GetNormal());
		ppc->flags = particleContact->GetFlags();
		ppc->type = ParticleContact::Begin;
		++particleContactNum;
	}

	void Engine::registerContact(b2ParticleSystem* particleSystem, int32 indexA, int32 indexB) {//particle-particle end
		VBE_WARN(particleContactNum < MAX_CONCURRENT_PARTICLECONTACTS, "Physics engine: Maximum concurrent particle contacts limit reached. Please raise limit or prevent contacts");
		if(particleContactNum == MAX_CONCURRENT_PARTICLECONTACTS) Log::warning() << "p" << Log::Flush; return;
		ParticleContact* ppc = &particleContactBuffer[particleBodyContactNum];
		ppc->sys = particleSystemBindings.at(particleSystem);
		ppc->a = indexA;
		ppc->b = indexB;
		ppc->type = ParticleContact::End;
		++particleContactNum;
	}

	void Engine::processContacts() {
		for(int i = 0; i < bodyContactNum; ++i) {
			BodyContact* contact = &bodyContactBuffer[i];
			if(deadBodyList.find(contact->a) != deadBodyList.end() || deadBodyList.find(contact->b) != deadBodyList.end()) continue;
			Body* pa = contact->a->getBody();
			Body* pb = contact->b->getBody();
			switch(contact->type) {
				case BodyContact::Begin:
					pa->onCollisionEnter(pa->getColliderIndex(contact->a), contact);
					std::swap(contact->a, contact->b);
					pb->onCollisionEnter(pb->getColliderIndex(contact->a), contact);
					break;
				case BodyContact::End:
					pa->onCollisionExit(pa->getColliderIndex(contact->a), contact);
					std::swap(contact->a, contact->b);
					pb->onCollisionExit(pb->getColliderIndex(contact->a), contact);
					break;
				case BodyContact::Stay:
					pa->onCollision(pa->getColliderIndex(contact->a), contact);
					std::swap(contact->a, contact->b);
					pb->onCollision(pb->getColliderIndex(contact->a), contact);
					break;
				case BodyContact::Null:
					VBE_ASSERT(contact->type != BodyContact::Null, "Null contact in contact list");
					break;
			}
		}
		bodyContactNum = 0;
		particleBodyContactNum = 0;
		for(int i = 0; i < particleBodyContactNum; ++i) {
			ParticleBodyContact* contact = &particleBodyContactBuffer[i];
			if(deadBodyList.find(contact->collider) != deadBodyList.end() || particleSystemBindings.find(contact->sys->sys) == particleSystemBindings.end()) continue;
			Body* b = contact->collider->getBody();
			switch(contact->type) {
				case ParticleBodyContact::Begin:
					contact->sys->onBodyCollisionEnter(contact);
					b->onParticleCollisionEnter(contact);
					break;
				case ParticleBodyContact::End:
					contact->sys->onBodyCollisionExit(contact->getParticleIndex(), contact->collider);
					b->onParticleCollisionExit(contact->sys, contact->getParticleIndex());
					break;
				case ParticleBodyContact::Null:
					VBE_ASSERT(contact->type != ParticleBodyContact::Null, "Null contact in contact list");
					break;
			}
		}
		for(int i = 0; i < particleBodyContactNum; ++i) {
			ParticleContact* contact = &particleContactBuffer[i];
			if(particleSystemBindings.find(contact->sys->sys) == particleSystemBindings.end()) continue;
			switch(contact->type) {
				case ParticleContact::Begin:
					contact->sys->onParticleCollisionEnter(contact);
					break;
				case ParticleContact::End:
					contact->sys->onParticleCollisionExit(contact->getParticleIndexA(), contact->getParticleIndexB());
					break;
				case ParticleContact::Null:
					VBE_ASSERT(contact->type != ParticleContact::Null, "Null contact in contact list");
					break;
			}
		}
		particleContactNum = 0;
	}

	Engine::EngineQueryCallback::EngineQueryCallback() {
	}

	Engine::EngineQueryCallback::~EngineQueryCallback() {
	}

	bool Engine::EngineQueryCallback::ReportFixture(b2Fixture* fixture) {
		Collider* collider = (Collider*) fixture->GetBody()->GetUserData();
		VBE_WARN((collider != nullptr), "An orphan collider has showed up on an AABB query. Orphaned colliders cannot be queried on AABB queries");
		return callback->reportCollider(collider);
	}

	bool Engine::EngineQueryCallback::ReportParticle(const b2ParticleSystem* particleSystem, int32 index) {
		ParticleSystem* sys = particleSystemBindings.at(particleSystem);
		VBE_WARN((sys != nullptr), "An orphan particleSystem has showed up on an AABB query. Orphaned colliders cannot be queried on AABB queries");
		return callback->reportParticle(sys, index);
	}

	bool Engine::EngineQueryCallback::ShouldQueryParticleSystem(const b2ParticleSystem* particleSystem) {
		ParticleSystem* sys = particleSystemBindings.at(particleSystem);
		VBE_WARN((sys != nullptr), "An orphan particleSystem has showed up on an AABB query. Orphaned colliders cannot be queried on AABB queries");
		return callback->shouldQueryParticleSystem(sys);
	}

	void Engine::EngineQueryCallback::reset(QueryCallback* newCallback) {
		callback = newCallback;
	}

	QueryCallback::QueryCallback() {
	}

	QueryCallback::~QueryCallback() {
	}

	Engine::CollisionCallback::CollisionCallback() {
	}

	Engine::CollisionCallback::~CollisionCallback() {
	}

	void Engine::CollisionCallback::BeginContact(b2Contact *contact) {
		Engine::registerContact(contact, BodyContact::Begin);
	}

	void Engine::CollisionCallback::EndContact(b2Contact *contact) {
		Engine::registerContact(contact, BodyContact::End);
	}

	void Engine::CollisionCallback::BeginContact(b2ParticleSystem* particleSystem, b2ParticleBodyContact* particleBodyContact) {
		Engine::registerContact(particleSystem, particleBodyContact);
	}

	void Engine::CollisionCallback::EndContact(b2Fixture* fixture, b2ParticleSystem* particleSystem, int32 index) {
		Engine::registerContact(fixture, particleSystem, index);
	}

	void Engine::CollisionCallback::BeginContact(b2ParticleSystem* particleSystem, b2ParticleContact* particleContact) {
		Engine::registerContact(particleSystem, particleContact);
	}

	void Engine::CollisionCallback::EndContact(b2ParticleSystem* particleSystem, int32 indexA, int32 indexB) {
		Engine::registerContact(particleSystem, indexA, indexB);
	}

	void Engine::CollisionCallback::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
		(void) oldManifold;
		Engine::registerContact(contact, BodyContact::Stay);
	}

	void Engine::CollisionCallback::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
		(void) contact; (void) impulse;
	}

}
