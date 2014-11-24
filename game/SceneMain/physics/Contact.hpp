#ifndef CONTACT_HPP
#define CONTACT_HPP

#include "commons.hpp"

namespace Physics {

	struct ContactPoint final {
			vec2f position;
			float normalImpulse, tangentImpulse, separation;
	};

	class Collider;
	class ParticleSystem;
	class BodyContact final {
		public:
			enum Type {
				Null = 0,
				Begin,
				Stay,
				End
			};

			BodyContact() : a(nullptr), b(nullptr), normal(0.0f), pointCount(0), type(Null) {}
			~BodyContact() {}

			Collider* getColliderA() const {return a;}
			Collider* getColliderB() const {return b;}
			vec2f getNormal() const {return normal;}
			int getPointCount() const {return pointCount;}
			const ContactPoint& getContactPoint(int index) const {
				VBE_ASSERT(index < pointCount, "That point does not exist in this contact");
				return points[index];
			}

		private:
			friend class Engine;

			Collider* a;
			Collider* b;
			vec2f normal;
			int pointCount;
			ContactPoint points[b2_maxManifoldPoints];
			Type type;
	};

	class ParticleBodyContact final {
		public:
			enum Type {
				Null = 0,
				Begin,
				End
			};

			ParticleBodyContact() : sys(nullptr), particle(0), normal(0.0f), weight(0.0f), mass(0.0f), type(Null) {}
			~ParticleBodyContact() {}

			int getParticleIndex() const {return particle;}
			ParticleSystem* getSystem() const {return sys;}
			Collider* getCollider() const {return collider;}
			vec2f getNormal() const {return normal;}
			float getWeight() const {return weight;}
			float getMass() const {return mass;}

		private:
			friend class Engine;

			ParticleSystem* sys;
			int particle;
			Collider* collider;
			vec2f normal;
			float weight, mass;
			Type type;
	};

	class ParticleContact final {
		public:
			enum Type {
				Null = 0,
				Begin,
				End
			};

			ParticleContact() : sys(nullptr), a(0), b(0), normal(0.0f), weight(0.0f), flags(0), type(Null) {}
			~ParticleContact() {}

			int getParticleIndexA() const {return a;}
			int getParticleIndexB() const {return b;}
			ParticleSystem* getSystem() const {return sys;}
			vec2f getNormal() const {return normal;}
			float getWeight() const {return weight;}
			unsigned int getFlags() const {return flags;}

		private:
			friend class Engine;

			ParticleSystem* sys;
			int a, b;
			vec2f normal;
			float weight;
			unsigned int flags;
			Type type;
	};

}

#endif // CONTACT_HPP
