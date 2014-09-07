#ifndef PHYSICSENGINE_HPP
#define PHYSICSENGINE_HPP
#include "Contact.hpp"

#define MAX_CONCURRENT_BODYCONTACTS 8192 //2**13
#define MAX_CONCURRENT_PARTICLEBODYCONTACTS 8192 //2**13
#define MAX_CONCURRENT_PARTICLECONTACTS 8192 //2**13

namespace Physics {

	class Body;
	class DebugDrawer;
	class ParticleSystem;
	class QueryCallback {
		public:
			QueryCallback();
			virtual ~QueryCallback();

			virtual bool reportCollider(Collider* collider) = 0;
			virtual bool reportParticle(ParticleSystem* system, int index) = 0;
			virtual bool shouldQueryParticleSystem(ParticleSystem* system) = 0;
	};

	class Engine {
		public:
			static void init(vec2f gravity = vec2f(0.0f, -9.8f), float step = 60.0f, int velIters = 2, int posIters = 8, int particleIters = 1);
			static void update(float deltaTime);
			static void draw(const DebugDrawer* drawer);
			static void drawGrid(const DebugDrawer* drawer, vec2f min, vec2f max, float step, vec4f color);
			static void queryAABB(QueryCallback* callback, AABB aabb);
			static void close();

			static b2World* world;
		private:
			class EngineQueryCallback : public b2QueryCallback {
				public:
					EngineQueryCallback();
					~EngineQueryCallback();
					bool ReportFixture(b2Fixture *fixture);
					bool ReportParticle(const b2ParticleSystem* particleSystem, int32 index);
					bool ShouldQueryParticleSystem(const b2ParticleSystem* particleSystem);
					void reset(QueryCallback* newCallback);

					QueryCallback* callback;
			};

			class CollisionCallback : public b2ContactListener {
				public:
					CollisionCallback();
					~CollisionCallback();

					void BeginContact(b2Contact* contact);
					void EndContact(b2Contact* contact);
					void BeginContact(b2ParticleSystem* particleSystem, b2ParticleBodyContact* particleBodyContact);
					void EndContact(b2Fixture* fixture, b2ParticleSystem* particleSystem, int32 index);
					void BeginContact(b2ParticleSystem* particleSystem, b2ParticleContact* particleContact);
					void EndContact(b2ParticleSystem* particleSystem, int32 indexA, int32 indexB);
					void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
					void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {(void) contact; (void) impulse;}
			};

			static b2Body* createBody();
			static void deleteBody(b2Body* body);
			static b2ParticleSystem* createParticleSystem(ParticleSystem* ppsys);
			static void deleteParticleSystem(b2ParticleSystem* sys);
			static b2Joint* createJoint(b2JointDef* d);
			static void deleteJoint(b2Joint* j);

			static void registerContact(b2Contact* c, BodyContact::Type t);
			static void registerContact(b2ParticleSystem* particleSystem, b2ParticleBodyContact* particleBodyContact);
			static void registerContact(b2Fixture* fixture, b2ParticleSystem* particleSystem, int32 index);
			static void registerContact(b2ParticleSystem* particleSystem, b2ParticleContact* particleContact);
			static void registerContact(b2ParticleSystem* particleSystem, int32 indexA, int32 indexB);
			static void processContacts();

			static float timestep;
			static int velocityIterations;
			static int positionIterations;
			static int particleIterations;
			static float accumulated;
			static EngineQueryCallback queryCallback;
			static CollisionCallback collisionCallback;
			static BodyContact* bodyContactBuffer;
			static ParticleBodyContact* particleBodyContactBuffer;
			static ParticleContact* particleContactBuffer;
			static int bodyContactNum;
			static int particleBodyContactNum;
			static int particleContactNum;
			static std::map<const b2ParticleSystem*, ParticleSystem*> particleSystemBindings;
			static std::set<Collider*> deadBodyList;

			friend class Collider;
			friend class CollisionCallback;
			friend class ParticleSystem;
			friend class Joint;
			friend class WeldJoint;

			Engine() {}
			~Engine() {}
	};

}

#endif // PHYSICSENGINE_HPP
