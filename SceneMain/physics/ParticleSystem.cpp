#include "ParticleSystem.hpp"
#include "Engine.hpp"

namespace Physics {

	ParticleSystem::ParticleSystem() : sys(nullptr) {
		sys = Engine::createParticleSystem(this);
	}

	ParticleSystem::~ParticleSystem() {
		Engine::deleteParticleSystem(sys);
		sys = nullptr;
	}

	void ParticleSystem::update(float deltaTime) {
		(void) deltaTime;
	}

	void ParticleSystem::draw() const {
	}

	void ParticleSystem::onParticleCollisionEnter(const ParticleContact* contact) {
		(void) contact;
	}

	void ParticleSystem::onParticleCollisionExit(int indexA, int indexB) {
		(void) indexA;
		(void) indexB;
	}

	void ParticleSystem::onBodyCollisionEnter(const ParticleBodyContact* contact) {
		(void) contact;
	}

	void ParticleSystem::onBodyCollisionExit(int index, Collider* collider) {
		(void) index;
		(void) collider;
	}

	int ParticleSystem::createParticle(const ParticleDef& def) {
		b2ParticleDef def2;
		def2.color = b2ParticleColor(def.color.x, def.color.y, def.color.z, def.color.w);
		def2.flags = def.flags;
		def2.lifetime = def.lifetime;
		def2.position = Utils::GLMv2ToB2Dv2(def.position);
		def2.velocity = Utils::GLMv2ToB2Dv2(def.velocity);
		return sys->CreateParticle(def2);
	}

	void ParticleSystem::destroyParticle(int index, bool callDestructionListener) {
		sys->DestroyOldestParticle(index, callDestructionListener);
	}

	void ParticleSystem::destroyOldestParticle(int index, bool callDestructionListener) {
		sys->DestroyOldestParticle(index, callDestructionListener);
	}

	int ParticleSystem::getParticleCount() const {
		return sys->GetParticleCount();
	}

	int ParticleSystem::getMaxParticleCount() const {
		return sys->GetMaxParticleCount();
	}

	void ParticleSystem::setMaxParticleCount(int count) {
		sys->SetMaxParticleCount(count);
	}

	unsigned int ParticleSystem::GetAllParticleFlags() const {
		return sys->GetAllParticleFlags();
	}

	bool ParticleSystem::getPaused() const {
		return sys->GetPaused();
	}

	void ParticleSystem::setDensity(float density) {
		sys->SetDensity(density);
	}

	float ParticleSystem::getDensity() const {
		return sys->GetDensity();
	}

	void ParticleSystem::setGravityScale(float gravityScale) {
		sys->SetGravityScale(gravityScale);
	}

	float ParticleSystem::getGravityScale() const {
		return sys->GetGravityScale();
	}

	void ParticleSystem::setDamping(float damping) {
		sys->SetDamping(damping);
	}

	float ParticleSystem::getDamping() const {
		return sys->GetDamping();
	}

	void ParticleSystem::setStaticPressureIterations(int iterations) {
		sys->SetStaticPressureIterations(iterations);
	}

	int ParticleSystem::getStaticPressureIterations() const {
		return sys->GetStaticPressureIterations();
	}

	void ParticleSystem::setRadius(float radius) {
		sys->SetRadius(radius);
	}

	float ParticleSystem::getRadius() const {
		return sys->GetRadius();
	}

	vec2f* ParticleSystem::getPositionBuffer() {
		return (vec2f*) sys->GetPositionBuffer();
	}

	const vec2f* ParticleSystem::getPositionBuffer() const {
		return (const vec2f*) sys->GetPositionBuffer();
	}

	vec2f* ParticleSystem::getVelocityBuffer() {
		return (vec2f*) sys->GetVelocityBuffer();
	}

	const vec2f* ParticleSystem::getVelocityBuffer() const {
		return (const vec2f*) sys->GetVelocityBuffer();
	}

	vec4uc*ParticleSystem::getColorBuffer() {
		return (vec4uc*) sys->GetColorBuffer();
	}

	const vec4uc* ParticleSystem::getColorBuffer() const {
		return (const vec4uc*) sys->GetColorBuffer();
	}

	float* ParticleSystem::GetWeightBuffer() {
		return sys->GetWeightBuffer();
	}

	const float* ParticleSystem::GetWeightBuffer() const {
		return sys->GetWeightBuffer();
	}

	void** ParticleSystem::getUserDataBuffer() {
		return sys->GetUserDataBuffer();
	}

	void* const*ParticleSystem::getUserDataBuffer() const {
		return sys->GetUserDataBuffer();
	}

	const unsigned int* ParticleSystem::getFlagsBuffer() const {
		return sys->GetFlagsBuffer();
	}

	void ParticleSystem::setParticleFlags(int index, unsigned int flags) {
		sys->SetParticleFlags(index, flags);
	}

	unsigned int ParticleSystem::getParticleFlags(int index) {
		return sys->GetParticleFlags(index);
	}

	void ParticleSystem::setFlagsBuffer(unsigned int* buffer, int capacity) {
		sys->SetFlagsBuffer(buffer, capacity);
	}

	void ParticleSystem::setPositionBuffer(vec2f* buffer, int capacity) {
		sys->SetPositionBuffer((b2Vec2*) buffer, capacity);
	}

	void ParticleSystem::setVelocityBuffer(vec2f* buffer, int capacity) {
		sys->SetVelocityBuffer((b2Vec2*) buffer, capacity);
	}

	void ParticleSystem::setColorBuffer(vec4uc* buffer, int capacity) {
		sys->SetColorBuffer((b2ParticleColor*) buffer, capacity);
	}

	void ParticleSystem::setUserDataBuffer(void** buffer, int capacity) {
		sys->SetUserDataBuffer(buffer, capacity);
	}

	void ParticleSystem::setStuckThreshold(int iterations) {
		sys->SetStuckThreshold(iterations);
	}

	const int*ParticleSystem::getStuckCandidates() const {
		return sys->GetStuckCandidates();
	}

	int ParticleSystem::getStuckCandidateCount() const {
		return sys->GetStuckCandidateCount();
	}

	float ParticleSystem::computeCollisionEnergy() const {
		return sys->ComputeCollisionEnergy();
	}

	void ParticleSystem::setStrictContactCheck(bool enabled) {
		sys->SetStrictContactCheck(enabled);
	}

	bool ParticleSystem::getStrictContactCheck() const {
		return sys->GetStrictContactCheck();
	}

	void ParticleSystem::setParticleLifetime(int index, float lifetime) {
		sys->SetParticleLifetime(index, lifetime);
	}

	float ParticleSystem::getParticleLifetime(int index) {
		return sys->GetParticleLifetime(index);
	}

	void ParticleSystem::setDestructionByAge(bool enable) {
		sys->SetDestructionByAge(enable);
	}

	bool ParticleSystem::getDestructionByAge() const {
		return sys->GetDestructionByAge();
	}

	const int* ParticleSystem::getExpirationTimeBuffer() {
		return sys->GetExpirationTimeBuffer();
	}

	float ParticleSystem::expirationTimeToLifetime(int expirationTime) const {
		return sys->ExpirationTimeToLifetime(expirationTime);
	}

	const int* ParticleSystem::getIndexByExpirationTimeBuffer() {
		return sys->GetIndexByExpirationTimeBuffer();
	}

	void ParticleSystem::particleApplyLinearImpulse(int index, const vec2f& impulse) {
		sys->ParticleApplyLinearImpulse(index, Utils::GLMv2ToB2Dv2(impulse));
	}

	void ParticleSystem::applyLinearImpulse(int firstIndex, int lastIndex, const vec2f& impulse) {
		sys->ApplyLinearImpulse(firstIndex, lastIndex, Utils::GLMv2ToB2Dv2(impulse));
	}

	void ParticleSystem::particleApplyForce(int index, const vec2f& force) {
		sys->ParticleApplyForce(index, Utils::GLMv2ToB2Dv2(force));
	}

	void ParticleSystem::applyForce(int firstIndex, int lastIndex, const vec2f& force) {
		sys->ApplyForce(firstIndex, lastIndex, Utils::GLMv2ToB2Dv2(force));
	}

}
