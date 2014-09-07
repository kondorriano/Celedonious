#ifndef MYDEBUGDRAWER_HPP
#define MYDEBUGDRAWER_HPP
#include "physics/DebugDrawer.hpp"

class DeferredContainer;
class MyDebugDrawer : public Physics::DebugDrawer {
	public:
		MyDebugDrawer();
		~MyDebugDrawer();

		void update(float deltaTime);
		void draw() const;

		void drawParticles(vec2f* centers, float radius, vec4uc* colors, int count);
		void drawPolygon(const vec2f* vertices, int vertexCount, const vec4f& color);
		void drawSolidPolygon(const vec2f* vertices, int vertexCount, const vec4f& color);
		void drawCircle(const vec2f& center, float radius, const vec4f& color);
		void drawSolidCircle(const vec2f& center, float radius, const vec2f& axis, const vec4f& color);
		void drawSegment(const vec2f& p1, const vec2f& p2, const vec4f& color);
		void drawTransform(const vec2f& position, const vec2f& xAxis, const vec2f& yAxis);

	private:
		bool drawEnabled;
		bool gridEnabled;
		Model circle;
		Model poly;
		DeferredContainer* renderer;
};

#endif // MYDEBUGDRAWER_HPP
