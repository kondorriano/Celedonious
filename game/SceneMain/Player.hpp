#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "commons.hpp"

class DeferredContainer;
class Player : public Physics::Body {
	public:

		enum ColliderID {
			Wheel = 0,
			Head = 1,
			Body = 2,
			JetPack = 3,
			Axis = 4,
			Sensor = 5,
		};

		enum Direction {
			Left = 1,
			Right = -1
		};

		Player();
		~Player();

	private:
		void update(float deltaTime);
		void movement(float deltaTime);
		void shoot(float deltaTime);
		void waterRefill(float deltaTime);
		void createParticle(vec2f position, vec2f direction);
		void draw() const;

		void onCollisionEnter(int colliderIndex, const Physics::BodyContact* contact);
		void onCollisionExit(int colliderIndex, const Physics::BodyContact* contact);
//		void onCollision(int colliderIndex, const Physics::BodyContact* contact);
//		void onParticleCollisionEnter(const Physics::ParticleBodyContact* contact);
//		void onParticleCollisionExit(Physics::ParticleSystem* system, int particleIndex);
		MeshBase* model;
		Camera* cam;
		vec3f pos;
		DeferredContainer* renderer;
		Direction dir;
		float jumpImpulse = 10.0f;
		float jumpImpulseCost = 2.0f; //2 seconds
		float jumpForce = 400.0f;
		float jumpForceCost = 5.0f; //5*deltatime
		float boostFrameCoolDown = 0.0f;
		bool colliding = false;
		float waterRefillVelocity = 7.5f;
		float waterTime = 10.0f;
		float waterCounter = 10.0f;
		bool usingWater = false;
};

#endif // PLAYER_HPP
