#ifndef FLUIDSIMULATION_HPP
#define FLUIDSIMULATION_HPP
#include "commons.hpp"

class Particle {
	public:
		static constexpr int MAX_NEIGHBORS = 75;

		Particle(vec2f position = vec2f(0.0f), vec2f velocity = vec2f(0.0f), bool alive = false);
		~Particle();

		vec2f position;
		vec2f velocity;
		bool alive;
		int index;
		float distances[MAX_NEIGHBORS];
		int neighbors[MAX_NEIGHBORS];
		int neighborCount;
		int ci;
		int cj;
};

class DeferredContainer;
class FluidSimulation : public GameObject {
	public:
		static constexpr int MAX_PARTICLES = 1000;
		static constexpr float RADIUS = 0.6f;
		static constexpr float VISCOSITY = 0.004f;
		static constexpr float DT = 1.0f / 60.0f;
		static constexpr float IDEAL_RADIUS = 50.0f;
		static constexpr float IDEAL_RADIUS_SQ = IDEAL_RADIUS*IDEAL_RADIUS;
		static constexpr float MULTIPLIER = IDEAL_RADIUS/RADIUS;
		static constexpr float CELL_SIZE = 0.5f;

		FluidSimulation();
		~FluidSimulation();

		void update(float deltaTime);
		void draw() const;

	private:
		void createParticle(vec2f pos, int numParticlesToSpawn);
		inline int getGridX(float x) { return floor(x/CELL_SIZE); }
		inline int getGridY(float y) { return floor(y/CELL_SIZE); }
		void applyLiquidConstraints();
		void findNeighbors(Particle* particle);

		int numActiveParticles;
		Particle liquid[MAX_PARTICLES];
		int activeParticles[MAX_PARTICLES];
		vec2f delta[MAX_PARTICLES];
		vec2f scaledPositions[MAX_PARTICLES];
		vec2f scaledVelocities[MAX_PARTICLES];
		std::map<int, std::map<int, std::list<int>>> grid;
		Model m;
		DeferredContainer* renderer;
};

#endif // FLUIDSIMULATION_HPP
