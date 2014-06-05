#include "FluidSimulation.hpp"
#include "DeferredContainer.hpp"
#include "physics/PhysicsBody.hpp"

constexpr int FluidSimulation::MAX_PARTICLES;
constexpr float FluidSimulation::RADIUS;
constexpr float FluidSimulation::VISCOSITY;
constexpr float FluidSimulation::DT;
constexpr float FluidSimulation::IDEAL_RADIUS;
constexpr float FluidSimulation::IDEAL_RADIUS_SQ;
constexpr float FluidSimulation::MULTIPLIER;
constexpr float FluidSimulation::CELL_SIZE;
constexpr int FluidSimulation::WORKER_THREADS;
constexpr int Particle::MAX_NEIGHBORS;
constexpr int Particle::MAX_FIXTURES_TO_TEST;

FluidSimulation::FluidSimulation() : TaskPool(WORKER_THREADS, [](){/*init function*/}), accumulatedTime(0.0f), numActiveParticles(0), renderer(nullptr) {
	renderer = (DeferredContainer*) getGame()->getObjectByName("deferred");
	std::vector<Vertex::Element> e;
	e.push_back(Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 2));
	Vertex::Format f(e);
	m.mesh = Mesh::loadEmpty(f,Mesh::DYNAMIC);
	m.program = Programs.get("debug");
	liquid = new Particle[MAX_PARTICLES];
	activeParticles = new int[MAX_PARTICLES];
	delta = new vec2f[MAX_PARTICLES];
	scaledPositions = new vec2f[MAX_PARTICLES];
	scaledVelocities = new vec2f[MAX_PARTICLES];
	simulationAABB = AABB(vec3f(-20.0f, -20.0f, 0.0f), vec3f(20.0f, 20.0f, 0.0f));
	for(int i = 0; i < MAX_PARTICLES; ++i) {
		liquid[i].index = i;
		activeParticles[i] = 0;
		delta[i] = vec2f(0.0f);
		scaledPositions[i] = vec2f(0.0f);
		scaledVelocities[i] = vec2f(0.0f);
	}
	VBE_LOG(   "FLUID SIMULATION INFO" << Log::Line
			   << "=====================" << Log::Line
			   << "DT: " << DT << Log::Line
			   << "MAX_PARTICLES: " << MAX_PARTICLES << Log::Line
			   << "RADIUS: " << RADIUS << Log::Line
			   << "VISCOSITY: " << VISCOSITY << Log::Line
			   << "IDEAL_RADIUS: " << IDEAL_RADIUS << Log::Line
			   << "IDEAL_RADIUS_SQ: " << IDEAL_RADIUS_SQ << Log::Line
			   << "MULTIPLIER: " << MULTIPLIER << Log::Line
			   << "CELL_SIZE: " << CELL_SIZE << Log::Line
			   << "WORKER_THREADS: " << WORKER_THREADS);
}

FluidSimulation::~FluidSimulation() {
}

void FluidSimulation::update(float deltaTime) {
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::P)) {createParticle(vec2f(cam->getWorldPos().x, cam->getWorldPos().y), 10);}
	accumulatedTime += deltaTime;
	while(accumulatedTime >= DT) {
		applyLiquidConstraints();
		accumulatedTime -= DT;
	}
}

void FluidSimulation::draw() const {
	if(renderer->getMode() != DeferredContainer::Forward) return;
	std::vector<vec2f> data;
	for(int i = 0; i < numActiveParticles; ++i) data.push_back(liquid[i].position);
	if(data.empty()) return;
	m.mesh->setVertexData(&data[0], data.size());
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	m.mesh->setPrimitiveType(Mesh::POINTS);
	m.program->uniform("color")->set(vec4f(1.0f, 1.0f, 1.0f, 0.5f));
	m.program->uniform("MVP")->set(cam->projection*cam->getView());
	m.draw();
}

void FluidSimulation::createParticle(vec2f pos, int numParticlesToSpawn){
	//search for dead particles to reinit
	std::list<Particle*> inactiveParticles;
	for(int j = 0;j < MAX_PARTICLES && int(inactiveParticles.size()) <=  numParticlesToSpawn; ++j)
		if(liquid[j].alive == false)
			inactiveParticles.push_back(&liquid[j]);
	for(std::list<Particle*>::iterator it = inactiveParticles.begin(); it != inactiveParticles.end(); ++it) {
		if (numActiveParticles < MAX_PARTICLES) {
			vec2f jitter = vec2f(((double)rand()/(RAND_MAX))*numParticlesToSpawn*0.5, ((double)rand()/(RAND_MAX))*numParticlesToSpawn*0.5);
			//reinit particle
			Particle* p = *it;
			p->position = pos + jitter;
			p->velocity = vec2f(0.0f,.1f);
			p->alive = true;
			p->ci = getGridX(p->position.x);
			p->cj = getGridY(p->position.y);
			p->neighborCount = 0;
			// Create grid cell if necessary
			if (grid.find(p->ci) == grid.end())
				grid[p->ci] = std::map<int, std::list<int>>();
			if (grid[p->ci].find(p->cj) == grid[p->ci].end())
				grid[p->ci][p->cj] = std::list<int>();
			grid[p->ci][p->cj].push_back(p->index);
			//set as active
			activeParticles[numActiveParticles] = p->index;
			numActiveParticles++;
		}
	}
}

void FluidSimulation::applyLiquidConstraints() {
	//PARALLEL
	for (int i = 0; i < numActiveParticles; i++) enqueue([this, i]() {prepareSimulation(activeParticles[i]);});
	wait(); //wait for all parallel jobs to end
	prepareCollisions();
	for (int i = 0; i < numActiveParticles; i++) enqueue([this, i]() {calculatePressure(activeParticles[i]);});
	wait();
	for (int i = 0; i < numActiveParticles; i++) enqueue([this, i]() {calculateForce(activeParticles[i]);});
	wait();
	//NOT PARALLEL
	for (int i = 0; i < numActiveParticles; i++) moveParticle(activeParticles[i]);
}

bool FluidSimulation::reportBody(PhysicsBody* body)  {
	AABB aabb = body->getCollider()->getAABB();
	// Get the top left corner of the AABB in grid coordinates
	int Ax = getGridX(aabb.getMax().x);
	int Ay = getGridY(aabb.getMin().y);

	// Get the bottom right corner of the AABB in grid coordinates
	int Bx = getGridX(aabb.getMax().x) + 1;
	int By = getGridY(aabb.getMax().y) + 1;
	// Loop through all the grid cells in the fixture's AABB
	for (int x = Ax; x < Bx; x++) {
		for (int y = Ay; y < By; y++) {
			if (grid.find(x) != grid.end() && grid[x].find(y) != grid[x].end()) {
				// Tell any particles we find that this fixture should be tested
				for (std::list<int>::iterator k = grid[x][y].begin(); k != grid[x][y].end(); k++) {
					Particle* particle = &liquid[*k];
					if (particle->numFixturesToTest < Particle::MAX_FIXTURES_TO_TEST) {
						particle->fixturesToTest[particle->numFixturesToTest] = body;
						particle->numFixturesToTest++;
					}
				}
			}
		}
	}
	return true;
}

void FluidSimulation::prepareCollisions()  {
	PhysicsEngine::queryAABB(this, simulationAABB);
}

void FluidSimulation::prepareSimulation(int index) {
	Particle* particle = &liquid[index];
	// Find neighbors
	findNeighbors(particle);
	// Scale positions and velocities
	scaledPositions[index] = particle->position * MULTIPLIER;
	scaledVelocities[index] = particle->velocity * MULTIPLIER;
	// Reset deltas
	delta[index] = vec2f(0.0f);
	for(int i = 0; i < particle->neighborCount; ++i)
		particle->neighborsDelta[i] = vec2f(0.0f);
	//Reset pressures
	particle->p = 0.0f;
	particle->pnear = 0.0f;
	particle->numFixturesToTest = 0;
}

void FluidSimulation::calculatePressure(int index) {
	Particle* particle = &liquid[index];
	for (int a = 0; a < particle->neighborCount; a++) {
		vec2f relativePosition = scaledPositions[particle->neighbors[a]] - scaledPositions[index];
		float distanceSq = glm::length(relativePosition)*glm::length(relativePosition);
		if (distanceSq < IDEAL_RADIUS_SQ) {
			particle->distances[a] = std::sqrt(distanceSq);
			if (particle->distances[a] < EPSILON) particle->distances[a] = IDEAL_RADIUS - .01f;
			float oneminusq = 1.0f - (particle->distances[a] / IDEAL_RADIUS);
			particle->p = (particle->p + oneminusq * oneminusq);
			particle->pnear = (particle->pnear + oneminusq * oneminusq * oneminusq);
		}
		else particle->distances[a] = std::numeric_limits<float>::max();
	}
}

void FluidSimulation::calculateForce(int index) {
	Particle* particle = &liquid[index];
	float pressure = (particle->p - 5.0f) / 2.0f; //normal pressure term
	float presnear = particle->pnear / 2.0f; //near particles term
	vec2f change(0.0f);
	for (int a = 0; a < particle->neighborCount; a++) {
		vec2f relativePosition = scaledPositions[particle->neighbors[a]] - scaledPositions[index];
		if (particle->distances[a] < IDEAL_RADIUS) {
			float q = particle->distances[a] / IDEAL_RADIUS;
			float oneminusq = 1.0f - q;
			float factor = oneminusq * (pressure + presnear * oneminusq) / (2.0f * particle->distances[a]);
			vec2f d = relativePosition * factor;
			vec2f relativeVelocity = scaledVelocities[particle->neighbors[a]] - scaledVelocities[index];
			factor = VISCOSITY * oneminusq * DT;
			d -= relativeVelocity * factor;
			particle->neighborsDelta[a] += d;
			change -= d;
		}
	}

	std::unique_lock<std::mutex> lock(deltaMutex);
	for (int j = 0; j < particle->neighborCount; j++)
		delta[particle->neighbors[j]] += particle->neighborsDelta[j] / MULTIPLIER;
	delta[index] += change / MULTIPLIER;
	particle->velocity += vec2f(0.0f, -9.8f/50.0f)*DT;
	lock.unlock();
}

void FluidSimulation::moveParticle(int index) {
	Particle* particle = &liquid[index];
	//apply delta// Update velocity
	particle->velocity += delta[index];
	// Update position
	particle->position += delta[index];
	particle->position += particle->velocity;
	//recalculate grid position
	int x = getGridX(particle->position.x);
	int y = getGridY(particle->position.y);
	if (particle->ci == x && particle->cj == y) return;
	//remove from grid and erase cells if necessary
	grid[particle->ci][particle->cj].remove(index);
	if (grid[particle->ci][particle->cj].size() == 0) {
		grid[particle->ci].erase(particle->cj);
		if (grid[particle->ci].size() == 0) grid.erase(particle->ci);
	}
	//reinsert into grid, create cells if necessary
	if (grid.find(x) == grid.end())
		grid[x] = std::map<int, std::list<int>>();
	if (grid[x].find(y) == grid[x].end())
		grid[x][y] = std::list<int>();
	grid[x][y].push_back(index);
	//apply changes to particle
	particle->ci = x;
	particle->cj = y;
}

void FluidSimulation::findNeighbors(Particle* particle) {
	particle->neighborCount = 0;
	for (int nx = -1; nx < 2; nx++) {
		for (int ny = -1; ny < 2; ny++) {
			int x = particle->ci + nx;
			int y = particle->cj + ny;
			if (grid.find(x) != grid.end() && grid[x].find(y) != grid[x].end()) {
				for (std::list<int>::iterator it = grid[x][y].begin(); it != grid[x][y].end(); ++it) {
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
	alive(alive),
	neighborCount(0.0f),
	p(0.0f),
	pnear(0.0f) {
	neighbors = new int[MAX_NEIGHBORS];
	distances = new float[MAX_NEIGHBORS];
	neighborsDelta = new vec2f[MAX_NEIGHBORS];
	fixturesToTest = new PhysicsBody*[MAX_FIXTURES_TO_TEST];
	for(int i = 0; i < MAX_NEIGHBORS; ++i) {neighbors[i] = 0; distances[i] = 0.0f; neighborsDelta[i] = vec2f(0.0f);}
}

Particle::~Particle() {
	delete neighbors;
	delete distances;
	delete neighborsDelta;
}
