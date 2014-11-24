#ifndef CIRCLECOLLIDER_HPP
#define CIRCLECOLLIDER_HPP
#include "Collider.hpp"

namespace Physics {

	class CircleCollider final : public Collider {
		public:
			CircleCollider();
			~CircleCollider() override;
	};

}

#endif // CIRCLECOLLIDER_HPP
