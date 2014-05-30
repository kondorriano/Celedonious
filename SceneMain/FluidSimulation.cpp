#include "FluidSimulation.hpp"
#include "DeferredContainer.hpp"
constexpr int FluidSimulation::MAX_PARTICLES;
constexpr float FluidSimulation::RADIUS;
constexpr float FluidSimulation::VISCOSITY;
constexpr float FluidSimulation::DT;
constexpr float FluidSimulation::IDEAL_RADIUS;
constexpr float FluidSimulation::IDEAL_RADIUS_SQ;
constexpr float FluidSimulation::MULTIPLIER;
constexpr float FluidSimulation::CELL_SIZE;
constexpr int Particle::MAX_NEIGHBORS;

FluidSimulation::FluidSimulation() : numActiveParticles(0), renderer(nullptr) {
	renderer = (DeferredContainer*) getGame()->getObjectByName("deferred");
	std::vector<Vertex::Element> e = {Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 2)};
	Vertex::Format f(e);
	m.mesh = Mesh::loadEmpty(f,Mesh::DYNAMIC);
	m.program = Programs.get("debug");
	for(int i = 0; i < MAX_PARTICLES; ++i) liquid[i].index = i;
}

FluidSimulation::~FluidSimulation() {
}

void FluidSimulation::update(float deltaTime) {
	(void) deltaTime;
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	if(Environment::getMouse()->isButtonHeld(Mouse::Left)) createParticle(vec2f(cam->getWorldPos().x, cam->getWorldPos().y), 1);
	applyLiquidConstraints();
}

void FluidSimulation::draw() const {
	if(renderer->getMode() != DeferredContainer::Forward) return;
	std::vector<vec2f> data;
	for(int i = 0; i < numActiveParticles; ++i) data.push_back(liquid[i].position);
	if(data.empty()) return;
	m.mesh->setVertexData(&data[0], data.size());
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	m.mesh->setPrimitiveType(Mesh::POINTS);
	m.program->uniform("color")->set(vec3f(1.0f));
	m.program->uniform("MVP")->set(cam->projection*cam->getView());
	m.draw();
}

void FluidSimulation::createParticle(vec2f pos, int numParticlesToSpawn){
	std::list<Particle*> inactiveParticles;
	int j = 0;
	for(int i = 0; i < numParticlesToSpawn; ++i)
		for(;j < MAX_PARTICLES; ++j)
			if(liquid[j].alive == false) {
				inactiveParticles.push_back(&liquid[j]);
				break;
			}

	for(std::list<Particle*>::iterator it = inactiveParticles.begin(); it != inactiveParticles.end(); ++it) {
		if (numActiveParticles < MAX_PARTICLES) {
			vec2f jitter = vec2f(((double)rand()/(RAND_MAX))*0.1, ((double)rand()/(RAND_MAX))*0.1);
			Particle* p = *it;
			p->position = pos + jitter;
			p->velocity = vec2f(0.0f);
			p->alive = true;
			p->ci = getGridX(p->position.x);
			p->cj = getGridY(p->position.y);

			// Create grid cell if necessary
			if (grid.find(p->ci) == grid.end())
				grid[p->ci] = std::map<int, std::list<int>>();
			if (grid[p->ci].find(p->cj) == grid[p->ci].end())
				grid[p->ci][p->cj] = std::list<int>();
			grid[p->ci][p->cj].push_back(p->index);

			activeParticles[numActiveParticles] = p->index;
			numActiveParticles++;
		}
	}
}

void FluidSimulation::applyLiquidConstraints() {
	// Prepare simulation
	for (int i = 0; i < numActiveParticles; i++) {
		int index = activeParticles[i];
		Particle* p = &liquid[index];
		scaledPositions[index] = p->position * MULTIPLIER;
		scaledVelocities[index] = p->velocity * MULTIPLIER;
		delta[index] = vec2f(0.0f);
		findNeighbors(p);
	}

	for (int i = 0; i < numActiveParticles; i++) {
		int index = activeParticles[i];
		Particle* pt = &liquid[index];
		// Calculate pressure
		float p = 0.0f;
		float pnear = 0.0f;
		for (int a = 0; a < pt->neighborCount; a++) {
			vec2f relativePosition = scaledPositions[pt->neighbors[a]] - scaledPositions[index];
			float distanceSq = relativePosition.length()*relativePosition.length();
			if (distanceSq < IDEAL_RADIUS_SQ) {
				pt->distances[a] = relativePosition.length();
				if (pt->distances[a] < 0.001f) pt->distances[a] = IDEAL_RADIUS - 0.01f;
				float oneminusq = 1.0f - (pt->distances[a]/IDEAL_RADIUS);
				p = (p + oneminusq*oneminusq);
				pnear = (pnear + oneminusq*oneminusq*oneminusq);
			}
			else pt->distances[a] = 500000000;
		}
		// Apply forces
		float pressure = (p - 5.0f) / 2.0f; //normal pressure term
		float presnear = pnear / 2.0f; //near particles term
		vec2f change(0.0f);
		for (int a = 0; a < pt->neighborCount; a++) {
			vec2f relativePosition = scaledPositions[pt->neighbors[a]] - scaledPositions[index];
			if (pt->distances[a] < IDEAL_RADIUS) {
				float q = pt->distances[a] / IDEAL_RADIUS;
				float oneminusq = 1.0f - q;
				float factor = oneminusq * (pressure + presnear*oneminusq) / (2.0f * pt->distances[a]);
				vec2f d = relativePosition * factor;
				vec2f relativeVelocity = scaledVelocities[pt->neighbors[a]] - scaledVelocities[index];
				factor = VISCOSITY * oneminusq * DT;
				d -= relativeVelocity * factor;
				delta[pt->neighbors[a]] += d;
				change -= d;
			}
		}
		delta[index] += change;
	}
	// Move particles
	for (int i = 0; i < numActiveParticles; i++) {
		int index = activeParticles[i];
		Particle* p = &liquid[index];
		p->position += delta[index] / MULTIPLIER;
		p->velocity += delta[index] / (MULTIPLIER * DT);
		int x = getGridX(p->position.x);
		int y = getGridY(p->position.y);
		if (p->ci == x && p->cj == y)
			continue;
		else {
			grid[p->ci][p->cj].remove(index);
			if (grid[p->ci][p->cj].size() == 0) {
				grid[p->ci].erase(p->cj);
				if(grid[p->ci].size() == 0)	grid.erase(p->ci);
			}
			if (grid.find(x) == grid.end())
				grid[x] = std::map<int, std::list<int>>();
			if (grid[x].find(y) == grid[x].end())
				grid[x][y] = std::list<int>();
			grid[x][y].push_back(index);
			p->ci = x;
			p->cj = y;
		}
	}
}
void FluidSimulation::findNeighbors(Particle* particle) {
	particle->neighborCount = 0;
	std::map<int, std::map<int, std::list<int>>>::iterator gridX;
	std::map<int, std::list<int>>::iterator gridY;
	for (int nx = -1; nx < 2; nx++) {
		for (int ny = -1; ny < 2; ny++) {
			int x = particle->ci + nx;
			int y = particle->cj + ny;
			gridX = grid.find(x);
			gridY = (gridX == grid.end())? gridY : gridX->second.find(y);
			if (gridX != grid.end() && gridY != gridX->second.end()) {
				for (std::list<int>::iterator it = gridY->second.begin(); it != gridY->second.end(); ++it) {
					if (*it != particle->index) {
						particle->neighbors[particle->neighborCount] = *it;
						particle->neighborCount++;
						if (particle->neighborCount >= Particle::MAX_NEIGHBORS)
							return;
					}
				}
			}
		}
	}
}

Particle::Particle(vec2f position, vec2f velocity, bool alive) :
	position(position),
	velocity(velocity),
	alive(alive) {
}

Particle::~Particle() {
}
