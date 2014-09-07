#ifndef CIRCLECOLLIDER_HPP
#define CIRCLECOLLIDER_HPP
#include "Collider.hpp"

namespace Physics {

	class CircleCollider : public Collider {
		public:
			CircleCollider();
			~CircleCollider();

			float getRadius();
			void setRadius(float r);

			int getVertexCount() const;
			vec2f getVertex(int index) const;
	};

}

#endif // CIRCLECOLLIDER_HPP
