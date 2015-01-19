#ifndef CELEDONIOUSSYSTEM_HPP
#define CELEDONIOUSSYSTEM_HPP
#include "commons.hpp"

class CeledoniousSystem : public Physics::ParticleSystem {
	public:
		CeledoniousSystem();
		~CeledoniousSystem();

	private:
		void update(float deltaTime) override;

};

#endif // CELEDONIOUSSYSTEM_HPP
