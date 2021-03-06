#ifndef SCENEMAIN_HPP
#define SCENEMAIN_HPP
#include "commons.hpp"

class Chunk;
class SceneMain : public GameObject {
	public:
		SceneMain();
		~SceneMain();
		void update(float deltaTime);

	private:
		void loadResources();

		GameObject* wall;
};

#endif // SCENEMAIN_HPP
