#ifndef SCENEMAIN_HPP
#define SCENEMAIN_HPP
#include "commons.hpp"
#include "physics/Physics.hpp"

class Chunk;
class SceneMain : public GameObject {
	public:
		SceneMain();
		~SceneMain();
		void update(float deltaTime);

	private:
		void loadResources();
		float debugCounter;
		int fpsCount;

		int pCount;
		Physics::ParticleSystem* psys;
		GameObject* wall;
};

#endif // SCENEMAIN_HPP
