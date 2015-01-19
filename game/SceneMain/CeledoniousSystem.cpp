#include "CeledoniousSystem.hpp"
#include "CeledoniousProfiler.hpp"

CeledoniousSystem::CeledoniousSystem() {
}

CeledoniousSystem::~CeledoniousSystem() {
}

void CeledoniousSystem::update(float deltaTime) {
	CeledoniousProfiler::particleCount = getParticleCount();
}
