#ifndef DEBUGDRAWER_HPP
#define DEBUGDRAWER_HPP
#include "physics/PhysicsDebugDrawer.hpp"

class DeferredContainer;
class DebugDrawer : public PhysicsDebugDrawer {
	public:
		DebugDrawer();
		~DebugDrawer();

		void draw() const;

		void drawPolygon(const vec2f* vertices, int vertexCount, const vec3f& color);
		void drawSolidPolygon(const vec2f* vertices, int vertexCount, const vec3f& color);
		void drawCircle(const vec2f& center, float radius, const vec3f& color);
		void drawSolidCircle(const vec2f& center, float radius, const vec2f& axis, const vec3f& color);
		void drawSegment(const vec2f& p1, const vec2f& p2, const vec3f& color);
		void drawTransform(const vec2f& position, const vec2f& xAxis, const vec2f& yAxis);

	private:
		Model circle;
		Model poly;
		DeferredContainer* renderer;
};

#endif // DEBUGDRAWER_HPP
