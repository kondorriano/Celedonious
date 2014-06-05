#ifndef FLUIDSIMULATION_HPP
#define FLUIDSIMULATION_HPP
#include "physics/PhysicsEngine.hpp"
#include "TaskPool.hpp"

class PhysicsBody;
class Particle {
	public:
		static constexpr int MAX_NEIGHBORS = 75;
		static constexpr int MAX_FIXTURES_TO_TEST = 20;

		Particle(vec2f position = vec2f(0.0f), vec2f velocity = vec2f(0.0f), bool alive = false);
		~Particle();

		vec2f position;
		vec2f velocity;
		bool alive;
		int index;
		float* distances;
		int* neighbors;
		vec2f* neighborsDelta;
		PhysicsBody** fixturesToTest;
		int neighborCount;
		int numFixturesToTest;
		int ci;
		int cj;
		float p;
		float pnear;
};

class DeferredContainer;
class FluidSimulation : public GameObject, public TaskPool, public PhysicsQueryCallback {
	public:
		static constexpr int MAX_PARTICLES = 4000;
		static constexpr float RADIUS = 0.9f;
		static constexpr float VISCOSITY = 0.0004f;
		static constexpr float DT = 1.0f / 30.0f;
		static constexpr float IDEAL_RADIUS = 50.0f;
		static constexpr float IDEAL_RADIUS_SQ = IDEAL_RADIUS*IDEAL_RADIUS;
		static constexpr float MULTIPLIER = IDEAL_RADIUS/RADIUS;
		static constexpr float CELL_SIZE = 0.6f;
		static constexpr float EPSILON = 0.001f;
		static constexpr int WORKER_THREADS = 4;

		FluidSimulation();
		~FluidSimulation();

		void update(float deltaTime);
		void draw() const;
		void createParticle(vec2f pos, int numParticlesToSpawn);

	private:
		void applyLiquidConstraints();

		bool reportBody(PhysicsBody *body);
		inline int getGridX(float x) { return floor(x/CELL_SIZE); }
		inline int getGridY(float y) { return floor(y/CELL_SIZE); }
		void prepareCollisions();
		void prepareSimulation(int index);
		void calculatePressure(int index);
		void calculateForce(int index);
		void moveParticle(int index);
		void findNeighbors(Particle* particle);

		float accumulatedTime;
		int numActiveParticles;
		Particle* liquid;
		int* activeParticles;
		vec2f* delta;
		vec2f* scaledPositions;
		vec2f* scaledVelocities;
		std::map<int, std::map<int, std::list<int>>> grid;
		AABB simulationAABB;
		Model m;
		DeferredContainer* renderer;
		std::mutex deltaMutex;
};

#endif // FLUIDSIMULATION_HPP
