#ifndef PHYSICSDEBUGDRAWER_HPP
#define PHYSICSDEBUGDRAWER_HPP
#include "commons.hpp"

class PhysicsDebugDrawer : public GameObject, public b2Draw {
	public:
		PhysicsDebugDrawer() {}
		~PhysicsDebugDrawer() {}

		inline void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
			drawPolygon((const vec2f*) vertices, vertexCount, vec3f(color.r, color.g, color.b));
		}

		inline void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
			drawSolidPolygon((const vec2f*) vertices, vertexCount, vec3f(color.r, color.g, color.b));
		}

		inline void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
			drawCircle(vec2f(center.x,center.y), radius, vec3f(color.r, color.g, color.b));
		}

		inline void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
			drawSolidCircle(vec2f(center.x, center.y), radius, vec2f(axis.x, axis.y), vec3f(color.r, color.g, color.b));
		}

		inline void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
			drawSegment(vec2f(p1.x, p1.y), vec2f(p2.x, p2.y), vec3f(color.r, color.g, color.b));
		}
		inline void DrawTransform(const b2Transform& xf) {
			drawTransform(vec2f(xf.p.x, xf.p.y), vec2f(xf.q.GetXAxis().x, xf.q.GetXAxis().y), vec2f(xf.q.GetYAxis().x, xf.q.GetYAxis().y));
		}

		virtual void drawPolygon(const vec2f* vertices, int vertexCount, const vec3f& color) = 0;
		virtual void drawSolidPolygon(const vec2f* vertices, int vertexCount, const vec3f& color) = 0;
		virtual void drawCircle(const vec2f& center, float radius, const vec3f& color) = 0;
		virtual void drawSolidCircle(const vec2f& center, float radius, const vec2f& axis, const vec3f& color) = 0;
		virtual void drawSegment(const vec2f& p1, const vec2f& p2, const vec3f& color) = 0;
		virtual void drawTransform(const vec2f& position, const vec2f& xAxis, const vec2f& yAxis) = 0;
};

#endif // PHYSICSDEBUGDRAWER_HPP
